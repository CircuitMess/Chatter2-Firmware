#include <Chatter.h>
#include "LoRaService.h"
#include "ProfileService.h"
#include "../Storage/Storage.h"

LoRaService LoRa;

const uint8_t LoRaService::PacketHeader[4] = { 0xba, 0xaa, 0xad, 0xff };

//fast clear of queues
template<typename T>
void clearQueue( std::queue<T> &q )
{
	std::queue<T> empty;
	std::swap( q, empty );
}

LoRaService::LoRaService() : radio(new Module(RADIO_CS, RADIO_DIO1, RADIO_RST, RADIO_BUSY, Chatter.getSPILoRa())), task("LoRaService", LoRaService::taskFunc, 4096, this){

}

bool LoRaService::begin(){
	int state = radio.begin(868, 500, 9, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 22, 8, 0, false);

	if(state != RADIOLIB_ERR_NONE){
		printf("Radio begin err\n");
		return false;
	}

	radio.setCurrentLimit(140);

	LoRaRandom();
	copyEncKeys();

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
		service->LoRaReceive();
		service->LoRaSend();
		service->LoRaRandom();
		delay(1);
	}
}

void LoRaService::LoRaRandom(){
	randomMutex.lock();

	while(randos.size() < randomSize){
		randos.push(radio.randomByte());
	}

	randomMutex.unlock();
}

void LoRaService::LoRaReceive(){
	LoRaPacket packet;

	int state = radio.receive(reinterpret_cast<uint8_t*>(&packet), sizeof(LoRaPacket) - sizeof(void*));
	if(state != RADIOLIB_ERR_NONE){
		return;
	}

	bool match = true;
	for(int i = 0; i < sizeof(PacketHeader) / sizeof(PacketHeader[0]); i++){
		match = packet.header[i] == PacketHeader[i];
		if(!match) break;
	}

	if(!match){
		printf("Header mismatch\n");
		return;
	}

	uint8_t* data = static_cast<uint8_t*>(malloc(packet.size));
	if(data == nullptr){
		printf("Error allocating packet content buffer, size: %lu\n", packet.size);
		return;
	}

	state = radio.receive(data, packet.size);
	if(state != RADIOLIB_ERR_NONE){
		printf("Error receiving packet content: %d\n", state);
		free(data);
		return;
	}

	if(packet.receiver != ESP.getEfuseMac() && (packet.type != LoRaPacket::PAIR_REQ && packet.type != LoRaPacket::PAIR_BROADCAST && packet.type != LoRaPacket::PAIR_ACK)){
		printf("Packet not addressed to this device: %lu\n", packet.receiver);
		free(data);
		return;
	}


	if(packet.type != LoRaPacket::PAIR_REQ && packet.type != LoRaPacket::PAIR_BROADCAST){
		encKeyMutex.lock();
		if(encKeyMap.find(packet.sender) == encKeyMap.end()){
			encKeyMutex.unlock();
			printf("Unknown sender: %lu\n", packet.sender);
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

	free(data);
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

	int status = radio.transmit(reinterpret_cast<uint8_t*>(&packet), sizeof(LoRaPacket) - sizeof(void*));
	if(status != RADIOLIB_ERR_NONE){
		printf("Error sending packet header: %d\n", status);
		return;
	}

	delay(5);

	status = radio.transmit(reinterpret_cast<uint8_t*>(packet.content), packet.size);
	if(status != RADIOLIB_ERR_NONE){
		printf("Error sending packet content: %d\n", status);
		return;
	}

	free(packet.content);
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
}

