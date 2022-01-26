#include <Chatter.h>
#include "LoRaService.h"

LoRaService LoRa;

uint8_t key[8] = { 1, 2, 3, 4, 5, 6, 7 };

const uint8_t LoRaService::PacketHeader[4] = { 0xba, 0xaa, 0xad, 0xff };

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
	if(!inited) return rand() % max;

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
