#include <Chatter.h>
#include "LoRaService.h"

LoRaService LoRa;

uint8_t key[8] = { 1, 2, 3, 4, 5, 6, 7 };

const uint8_t LoRaService::PacketHeader[8] = { 0xba, 0xaa, 0xad, 0xff, 0xca, 0xff, 0xee, 0xa0 };
const uint8_t LoRaService::PacketTrailer[8] = { 0xab, 0xaa, 0xda, 0xff, 0xac, 0xff, 0xee, 0x0a };

bool LoRaService::available = false;

LoRaService::LoRaService() : radio(new Module(RADIO_CS, RADIO_DIO1, RADIO_RST, RADIO_BUSY, Chatter.getSPILoRa())),
task("LoRaService", LoRaService::taskFunc, 4096, this), inputBuffer(1024){

}

IRAM_ATTR void LoRaService::moduleInterrupt(){
	LoRaService::available = true;
}

bool LoRaService::begin(){
	int state = radio.begin(868, 500, 9, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 22, 8, 0, false);

	if(state != RADIOLIB_ERR_NONE){
		printf("Radio begin err\n");
		return false;
	}

	radio.setCurrentLimit(140);

	/*printf("entering dep sleep...\n");
	esp_sleep_enable_ext0_wakeup((gpio_num_t) RADIO_DIO1, 1);
	esp_deep_sleep_start();*/

	LoRaRandom();

	radio.setDio1Action(LoRaService::moduleInterrupt);

	printf("LoRa: Start receive... ");
	state = radio.startReceive();
	if(state == RADIOLIB_ERR_NONE){
		printf("Success!\n");
	}else{
		printf("Failed, error code %d\n", state);
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
		//service->LoRaRandom();
		while(available){
			service->LoRaReceive();
		}
		service->LoRaSend();
		delay(1);

		service->LoRaProcessBuffer();
		while(!service->received.empty()){
			LoRaPacket packet = service->received.front();
			service->received.pop();

			service->LoRaProcessPacket(packet);
			free(packet.content);
		}
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
	if(!available) return;
	available = false;

	size_t size = radio.getPacketLength();
	if(size == 0){
		radio.startReceive();
		return;
	}

	printf("LoRa: %ld available\n", size);

	if(inputBuffer.writeAvailable() < size){
		printf("LoRa: Input buffer full - %d B available, need %d B\n", inputBuffer.writeAvailable(), size);
		return;
	}

	uint8_t* data = static_cast<uint8_t*>(malloc(size));

	int state = radio.readData(data, size);
	if(state != RADIOLIB_ERR_NONE){
		printf("LoRa: Error %d receiving %ld bytes of data\n", state, size);
		free(data);
		return;
	}

	radio.startReceive();

	inputBuffer.write(data, size);
	free(data);
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

		uint8_t* buf = static_cast<uint8_t*>(malloc(offset));
		inputBuffer.read(buf, offset);
		free(buf);

		return;
	}

	const auto* contentSize = inputBuffer.peek<typeof(LoRaPacket::size)>(offsetof(LoRaPacket, size));
	if(contentSize == nullptr) return; // waiting for rest of packet

	auto cSize = *contentSize;
	if(cSize > 80){ // TODO: max content size define
		printf("LoRa: Content size too big: %ld\n", cSize);

		// Remove header so next iteration clears all data before next header is found
		uint8_t* buf = static_cast<uint8_t*>(malloc(offset));
		inputBuffer.read(buf, offset);
		free(buf);

		return;
	}

	size_t trailerPos = sizeof(LoRaPacket) - sizeof(void*) + cSize;
	if(trailerPos + sizeof(PacketTrailer) > inputBuffer.readAvailable()) return; // waiting for rest of packet

	bool trailerFound = checkSync(PacketTrailer, trailerPos) != SIZE_MAX;
	if(!trailerFound){
		printf("LoRa: trailer not found\n");

		// Remove header so next iteration clears all data before next header is found
		uint8_t* buf = static_cast<uint8_t*>(malloc(offset));
		inputBuffer.read(buf, offset);
		free(buf);

		return;
	}

	LoRaPacket packet;
	inputBuffer.read(reinterpret_cast<uint8_t*>(&packet), sizeof(LoRaPacket) - sizeof(void*));

	packet.content = malloc(packet.size);
	if(packet.content == nullptr){
		printf("LoRa: cannot allocate %ld B for packet content\n", packet.size);
		return;
	}

	inputBuffer.read(static_cast<uint8_t*>(packet.content), packet.size);

	uint8_t trailer[sizeof(PacketTrailer)];
	inputBuffer.read(trailer, sizeof(PacketTrailer));

	received.push(packet);

	printf("Received packet!\n");
	printf("Type %d\n", packet.type);
}

void LoRaService::LoRaProcessPacket(LoRaPacket& packet){
	uint8_t* data = static_cast<uint8_t*>(packet.content);
	for(size_t i = 0, j = 0; i < packet.size; i++, j = (j + 1) % sizeof(key)){
		data[i] = data[i] ^ key[j];
	}

	// TODO: checksum checking
	// TODO: profile hash checking
	// TODO: other message types

	if(packet.type == LoRaPacket::MSG){
		ReceivedPacket<MessagePacket> received;
		received.sender = packet.sender;
		received.content = MessagePacket::unpack(data);

		inboxMutex.lock();
		inbox.message.push(received);
		inboxMutex.unlock();
	}
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

	int status = radio.transmit(buf, size);
	if(status != RADIOLIB_ERR_NONE){
		printf("Error sending packet: %d\n", status);
	}

	free(packet.content);
	free(buf);

	radio.startReceive();
}

void LoRaService::send(UID_t receiver, LoRaPacket::Type type, const Packet* content){
	LoRaPacket packet;
	memcpy((void*) packet.header, PacketHeader, sizeof(PacketHeader));
	packet.sender = ESP.getEfuseMac();
	packet.receiver = receiver;
	packet.type = type;

	// TODO: checksum and profile hash
	packet.checksum = 1;
	packet.profileHash = 2;

	if(type == LoRaPacket::MSG){
		const MessagePacket& msgPacket = *reinterpret_cast<const MessagePacket*>(content);
		packet.size = msgPacket.pack(&packet.content);
	}else{
		return;
	}

	for(size_t i = 0, j = 0; i < packet.size; i++, j = (j + 1) % sizeof(key)){
		uint8_t* data = static_cast<uint8_t*>(packet.content);
		data[i] = data[i] ^ key[j];
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
