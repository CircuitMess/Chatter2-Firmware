#include <Chatter.h>
#include "LoRaService.h"
#include "ProfileService.h"
#include "../Storage/Storage.h"

LoRaService LoRa;

uint8_t key[8] = { 1, 2, 3, 4, 5, 6, 7 };

const uint8_t LoRaService::PacketHeader[8] = { 0xba, 0xaa, 0xad, 0xff, 0xca, 0xff, 0xee, 0xa0 };
const uint8_t LoRaService::PacketTrailer[8] = { 0xab, 0xaa, 0xda, 0xff, 0xac, 0xff, 0xee, 0x0a };

volatile bool LoRaService::available = false;
portMUX_TYPE LoRaService::mux = portMUX_INITIALIZER_UNLOCKED;

//fast clear of queues
template<typename T>
void clearQueue( std::queue<T> &q )
{
	std::queue<T> empty;
	std::swap( q, empty );
}

LoRaService::LoRaService() : radio(new Module(RADIO_CS, RADIO_DIO1, RADIO_RST, RADIO_BUSY, Chatter.getSPILoRa())),
task("LoRaService", LoRaService::taskFunc, 4096, this), inputBuffer(1024){

}

IRAM_ATTR void LoRaService::moduleInterrupt(){
	portENTER_CRITICAL(&mux);
	LoRaService::available = true;
	portEXIT_CRITICAL(&mux);
	// LoRa.radio.clearIrqStatus();
}

void LoRaService::initStateless(){
	Module* mod = radio.getMod();

	// SX126x::begin
	mod->init();
	mod->pinMode(mod->getIrq(), INPUT);
	mod->pinMode(mod->getGpio(), INPUT);

	radio._bwKhz = 500; // this
	radio._sf = 9; // this
	radio._bw = RADIOLIB_SX126X_LORA_BW_125_0;
	radio._cr = RADIOLIB_SX126X_LORA_CR_4_7;
	radio._ldro = 0x00;
	radio._crcType = RADIOLIB_SX126X_LORA_CRC_ON;
	radio._preambleLength = 8; // this
	radio._tcxoDelay = 0;
	radio._headerType = RADIOLIB_SX126X_LORA_HEADER_EXPLICIT;
	radio._implicitLen = 0xFF;

	radio.setSpreadingFactor(radio._sf);
	radio.setBandwidth(radio._bwKhz);
	radio.setCodingRate(5); // this
	radio.setPreambleLength(radio._preambleLength);

	available = digitalRead(mod->getIrq()) == HIGH;
}

bool LoRaService::begin(){
	int state = radio.begin(915, 500, 9, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 22, 8, 0, false);

	if(state != RADIOLIB_ERR_NONE){
		printf("LoRa: Radio begin err\n");
		return false;
	}

	radio.setCurrentLimit(140);

	/*printf("entering dep sleep...\n");
	esp_sleep_enable_ext0_wakeup((gpio_num_t) RADIO_DIO1, 1);
	esp_deep_sleep_start();*/

	LoRaRandom();
	copyEncKeys();

	radio.setDio1Action(LoRaService::moduleInterrupt);

	state = radio.startReceive();
	if(state != RADIOLIB_ERR_NONE){
		printf("LoRa: start failed, error code %d\n", state);
		return false;
	}

	task.start(1, 0);

	inited = true;
	return true;
}

int32_t LoRaService::rand(){
	return LoRaService::rand(INT32_MAX);
}

int32_t LoRaService::rand(int32_t min, int32_t max){
	if(min > max) return 0;
	return LoRaService::rand(max - min) + min;
}

int32_t LoRaService::rand(int32_t max){
	if(!inited) return ::rand() % max;

	size_t available;
	do {
		randomMutex.lock();
		available = randos.size();
		randomMutex.unlock();
	} while(available < 4);

	uint8_t bytes[4];
	randomMutex.lock();
	for(uint8_t i = 0; i < 4; i++) {
		bytes[i] = randos.front();
		randos.pop();
	}
	randomMutex.unlock();

	int32_t number = ((int32_t) bytes[0] << 24) | ((int32_t) bytes[1] << 16) | ((int32_t) bytes[2] << 8) | ((int32_t) bytes[3]);
	if(number < 0){
		number *= -1;
	}

	return number % max;
}

UID_t LoRaService::randUID(){
	UID_t upper = LoRaService::rand();
	UID_t lower = LoRaService::rand();
	return ((upper << 32) & 0xFFFFFFFF00000000) | (lower & 0xFFFFFFFF);
}

void LoRaService::taskFunc(Task* task){
	LoRaService* service = static_cast<LoRaService*>(task->arg);

	while(task->running){
		service->working = true;
		service->LoRaRandom();
		service->loop();
		service->working = false;
		delay(10);
	}
}

void LoRaService::loop(){
	LoRaReceive();
	LoRaSend();

	delay(1);

	LoRaProcessBuffer();
	LoRaProcessPackets();
}

void LoRaService::LoRaRandom(){
	randomMutex.lock();
	if(randos.size() == randomSize){
		randomMutex.unlock();
		return;
	}

	radio.setDio1Action(nullptr);

	while(randos.size() < randomSize){
		randos.push(radio.randomByte());
	}

	radio.setDio1Action(LoRaService::moduleInterrupt);
	radio.startReceive();
	randomMutex.unlock();
}

void LoRaService::LoRaReceive(){
	portENTER_CRITICAL(&mux);
	if(!available){
		portEXIT_CRITICAL(&mux);
		return;
	}
	portEXIT_CRITICAL(&mux);

#define err() portENTER_CRITICAL(&mux); available = false; portEXIT_CRITICAL(&mux); return

	size_t size = radio.getPacketLength();
	if(size == 0){
		err();
	}

	if(inputBuffer.writeAvailable() < size){
		printf("LoRa: Input buffer full - %d B available, need %d B\n", inputBuffer.writeAvailable(), size);
		err();
	}

	uint8_t* data = static_cast<uint8_t*>(malloc(size));

	int state = radio.readData(data, size);
	if(state != RADIOLIB_ERR_NONE){
		printf("LoRa: Error %d receiving %ld bytes of data\n", state, size);
		free(data);
		radio.startReceive();
		radio.setDio1Action(LoRaService::moduleInterrupt);
		err();
	}

	inputBuffer.write(data, size);
	free(data);

	radio.startReceive();
	radio.setDio1Action(LoRaService::moduleInterrupt);
	err();
}

void LoRaService::LoRaProcessBuffer(){
	if(inputBuffer.readAvailable() < 8) return;

	auto checkSync = [this](const uint8_t syncWord[8], size_t start = 0, size_t size = 0, bool offsetOnFail = false){
		if(size == 0){
			size = inputBuffer.readAvailable();
		}else{
			size = min(size, inputBuffer.readAvailable());
		}

		const size_t syncSize = 8;
		size_t matched = 0;
		size_t offset;
		for(offset = start; offset < size && matched < syncSize; offset++){
			const auto* p = inputBuffer.peek<const uint8_t>(offset);
			if(p == nullptr){
				printf("nullptr on byte %ld / %ld\n", offset, size);
				continue;
			}
			uint8_t c = *p;

			if(c == syncWord[matched]){
				matched++;

				if(matched == syncSize){
					break;
				}
			}else if(matched != 0){
				matched = 0;
			}
		}

		if(matched == syncSize) return offset - (syncSize - 1);
		else if (offsetOnFail) return size - matched;
		else return SIZE_MAX;
	};

	size_t offset = checkSync(PacketHeader);
	if(offset == SIZE_MAX){
		printf("LoRa: Header not found\n");

		offset = checkSync(PacketHeader, 0, 0, true);
		printf("LoRa: Clearing %ld / %ld B\n", offset, inputBuffer.readAvailable());

		inputBuffer.skip(offset);
		return;
	}else if(offset != 0){
		printf("LoRa: found %lu B before packet header\n", offset);
		inputBuffer.skip(offset);
	}

	const auto* contentSize = inputBuffer.peek<typeof(LoRaPacket::size)>(offsetof(LoRaPacket, size));
	if(contentSize == nullptr) return; // waiting for rest of packet

	auto cSize = *contentSize;
	if(cSize > 80){ // TODO: max content size define
		printf("LoRa: Content size too big: %lu\n", cSize);

		// Remove header so next iteration clears all data before next header is found
		inputBuffer.skip(sizeof(PacketHeader));
		return;
	}

	size_t trailerPos = sizeof(LoRaPacket) - sizeof(void*) + cSize;
	if(trailerPos + sizeof(PacketTrailer) > inputBuffer.readAvailable()) return; // waiting for rest of packet

	auto trailerFound = checkSync(PacketTrailer, trailerPos);
	if(trailerFound == SIZE_MAX){
		printf("LoRa: trailer not found\n");

		// Remove header so next iteration clears all data before next header is found
		inputBuffer.skip(sizeof(PacketHeader));
		return;
	}else if(trailerFound != trailerPos){
		printf("LoRa: trailer found after %lu B\n", trailerFound - trailerPos);

		// Remove header so next iteration clears all data before next header is found
		inputBuffer.skip(sizeof(PacketHeader));
		return;
	}

	LoRaPacket packet;
	inputBuffer.read(reinterpret_cast<uint8_t*>(&packet), sizeof(LoRaPacket) - sizeof(void*));

	packet.content = malloc(packet.size);
	if(packet.content == nullptr){
		printf("LoRa: cannot allocate %ld B for packet content\n", packet.size);
		free(packet.content);
		return;
	}

	inputBuffer.read(static_cast<uint8_t*>(packet.content), packet.size);

	if((trailerFound = checkSync(PacketTrailer)) != 0){
		if(trailerFound == SIZE_MAX){
			printf("LoRa: Trailer missmatch\n");
		}else{
			printf("LoRa: Trailer not right, offset by %ld\n", trailerFound);
		}
		free(packet.content);
		return;
	}

	uint8_t trailer[sizeof(PacketTrailer)];
	inputBuffer.read(trailer, sizeof(PacketTrailer));

	received.push(packet);
}

void LoRaService::LoRaProcessPackets(){
	while(!received.empty()){
		LoRaPacket packet = received.front();
		received.pop();

		LoRaProcessPacket(packet);
	}
}

void LoRaService::LoRaProcessPacket(LoRaPacket& packet){
	if(packet.sender == 0 || packet.sender == ESP.getEfuseMac()){
		printf("LoRa: Received own packet!\n");
		free(packet.content);
		return;
	}

	uint8_t* data = static_cast<uint8_t*>(packet.content);

	if((packet.receiver != ESP.getEfuseMac() && packet.receiver != 0) || (packet.receiver == 0 && packet.type != LoRaPacket::PAIR_BROADCAST)){
		free(data);
		return;
	}


	if(packet.type != LoRaPacket::PAIR_REQ && packet.type != LoRaPacket::PAIR_BROADCAST){
		encKeyMutex.lock();
		if(encKeyMap.find(packet.sender) == encKeyMap.end()){
			encKeyMutex.unlock();
			free(data);
			return;
		}
		uint8_t *encKey = encKeyMap[packet.sender];
		encKeyMutex.unlock();
		encDec(data, packet.size, encKey);
	}
	// TODO: checksum checking
	if(encKeyMap.find(packet.sender) != encKeyMap.end()){
		hashmapMutex.lock();
		hashMap[packet.sender] = packet.profileHash;
		hashmapMutex.unlock();
	}
	// TODO: other message types

	if(packet.type == LoRaPacket::MSG){
		ReceivedPacket<MessagePacket> received;
		received.sender = packet.sender;
		received.content = MessagePacket::unpack(data);

		inboxMutex.lock();
		inbox.message.push(received);
		inboxMutex.unlock();
	}else if(packet.type == LoRaPacket::PAIR_BROADCAST){
		ReceivedPacket<AdvertisePair> received;
		received.sender = packet.sender;
		received.content = AdvertisePair::unpack(data);

		inboxMutex.lock();
		inbox.pairBroadcast.push(received);
		inboxMutex.unlock();
	}else if(packet.type == LoRaPacket::PAIR_REQ){
		ReceivedPacket<RequestPair> received;
		received.sender = packet.sender;
		received.content = RequestPair::unpack(data);

		inboxMutex.lock();
		inbox.pairRequests.push(received);
		inboxMutex.unlock();
	}else if(packet.type == LoRaPacket::PAIR_ACK){
		ReceivedPacket<AckPair> received;
		received.sender = packet.sender;
		received.content = AckPair::unpack(data);

		inboxMutex.lock();
		inbox.pairAcks.push(received);
		inboxMutex.unlock();
	}else if(packet.type == LoRaPacket::PROF){
		ReceivedPacket<ProfilePacket> received;
		received.sender = packet.sender;
		received.content = ProfilePacket::unpack(data);

		inboxMutex.lock();
		inbox.profile.push(received);
		inboxMutex.unlock();
	}

	free(packet.content);
}

void LoRaService::LoRaSend(){
	outboxMutex.lock();
	if(outbox.empty()){
		outboxMutex.unlock();
		return;
	}

	LoRaPacket packet = outbox.front();
	outbox.pop();
	outboxMutex.unlock();

	size_t size = sizeof(LoRaPacket) - sizeof(void*) + packet.size + sizeof(PacketTrailer);
	uint8_t* buf = static_cast<uint8_t*>(malloc(size));

	memcpy(buf, &packet, sizeof(LoRaPacket) - sizeof(void*));
	memcpy(buf + sizeof(LoRaPacket) - sizeof(void*), packet.content, packet.size);
	memcpy(buf + sizeof(LoRaPacket) - sizeof(void*) + packet.size, PacketTrailer, sizeof(PacketTrailer));

	radio.setDio1Action(nullptr);
	int status = radio.transmit(buf, size);
	if(status != RADIOLIB_ERR_NONE){
		printf("Error sending packet: %d\n", status);
	}

	free(packet.content);
	free(buf);

	radio.startReceive();
	radio.setDio1Action(LoRaService::moduleInterrupt);
}

void LoRaService::send(UID_t receiver, LoRaPacket::Type type, const Packet* content){
	LoRaPacket packet;
	memcpy((void*) packet.header, PacketHeader, sizeof(PacketHeader));
	packet.sender = ESP.getEfuseMac();
	packet.receiver = receiver;
	packet.type = type;

	// TODO: checksum
	packet.checksum = 1;
	packet.profileHash = Profiles.getMyHash();

	packet.size = content->pack(&packet.content);
	if(packet.type != LoRaPacket::PAIR_REQ && packet.type != LoRaPacket::PAIR_BROADCAST){
		encKeyMutex.lock();
		if(encKeyMap.find(receiver) == encKeyMap.end()){
			encKeyMutex.unlock();
			printf("Recipient not found: %lu\n", receiver);
			free(packet.content);
			return;
		}
		uint8_t* encKey = encKeyMap[receiver];
		encKeyMutex.unlock();
		encDec(packet.content, packet.size, encKey);
	}

	outboxMutex.lock();
	outbox.push(packet);
	outboxMutex.unlock();
}

ReceivedPacket<MessagePacket> LoRaService::getMessage(){
	inboxMutex.lock();
	if(inbox.message.empty()){
		inboxMutex.unlock();
		return { 0, nullptr };
	}

	ReceivedPacket<MessagePacket> packet = inbox.message.front();
	inbox.message.pop();
	inboxMutex.unlock();

	return packet;
}

ReceivedPacket<ProfilePacket> LoRaService::getProfile(){
	inboxMutex.lock();
	if(inbox.profile.empty()){
		inboxMutex.unlock();
		return { 0, nullptr };
	}

	ReceivedPacket<ProfilePacket> packet = inbox.profile.front();
	inbox.profile.pop();
	inboxMutex.unlock();

	return packet;
}

std::map<UID_t, size_t>* LoRaService::getHashmapCopy(){
	hashmapMutex.lock();
	auto mapCopy = new std::map<UID_t, size_t>(hashMap);
	hashmapMutex.unlock();
	return mapCopy;
}

ReceivedPacket<AdvertisePair> LoRaService::getPairBroadcast(){
	inboxMutex.lock();
	if(inbox.pairBroadcast.empty()){
		inboxMutex.unlock();
		return { 0, nullptr };
	}

	ReceivedPacket<AdvertisePair> packet = inbox.pairBroadcast.front();
	inbox.pairBroadcast.pop();
	inboxMutex.unlock();

	return packet;
}

ReceivedPacket<RequestPair> LoRaService::getPairRequest(){
	inboxMutex.lock();
	if(inbox.pairRequests.empty()){
		inboxMutex.unlock();
		return { 0, nullptr };
	}

	ReceivedPacket<RequestPair> packet = inbox.pairRequests.front();
	inbox.pairRequests.pop();
	inboxMutex.unlock();

	return packet;
}

ReceivedPacket<AckPair> LoRaService::getPairAck(){
	inboxMutex.lock();
	if(inbox.pairAcks.empty()){
		inboxMutex.unlock();
		return { 0, nullptr };
	}

	ReceivedPacket<AckPair> packet = inbox.pairAcks.front();
	inbox.pairAcks.pop();
	inboxMutex.unlock();

	return packet;
}

void LoRaService::encDec(void* data, size_t size, const uint8_t* key){
	auto ptr = (uint8_t*)data;
	for(size_t i = 0, j = 0; i < size; i++, j = (j + 1) % 32){
		ptr[i] = ptr[i] ^ key[j];
	}
}

void LoRaService::copyEncKeys(){
	encKeyMutex.lock();
	encKeyMap.clear();
	for(auto &user : Storage.Friends.all()){
		if(user == ESP.getEfuseMac()) continue;

		memcpy(encKeyMap[user], Storage.Friends.get(user).encKey, 32);
	}

	hashmapMutex.lock();
	for(auto &user : hashMap){
		if(encKeyMap.find(user.first) == encKeyMap.end()){
			hashMap.erase(user.first);
		}
	}
	hashmapMutex.unlock();

	encKeyMutex.unlock();
}

void LoRaService::clearPairPackets(){
	inboxMutex.lock();
	clearQueue(inbox.pairRequests);
	clearQueue(inbox.pairBroadcast);
	clearQueue(inbox.pairAcks);
	inboxMutex.unlock();

	outboxMutex.lock();
	clearQueue(outbox);
	outboxMutex.unlock();
}

