#include "LoRaPacket.h"

size_t MessagePacket::pack(void** destination) const {
	size_t size = sizeof(uid) + sizeof(type);
	uint8_t* buffer = static_cast<uint8_t*>(malloc(size));

	memcpy(buffer, &type, sizeof(type));
	memcpy(buffer + sizeof(type), &uid, sizeof(uid));

	*destination = buffer;
	return size;
}

MessagePacket* MessagePacket::unpack(void* _buffer){
	uint8_t* buffer = static_cast<uint8_t*>(_buffer);

	MessagePacket* packet;
	Type type = *reinterpret_cast<Type*>(buffer);

	if(type == ACK){
		packet = new MessagePacket;
	}else if(type == TEXT){
		packet = TextMessage::unpack(buffer + sizeof(type) + sizeof(uid));
	}else if(type == PIC){
		packet = PicMessage::unpack(buffer + sizeof(type) + sizeof(uid));
	}else{
		return nullptr;
	}

	packet->type = type;
	packet->uid = *reinterpret_cast<UID_t*>(buffer + sizeof(Type));

	return packet;
}

TextMessage::TextMessage(){
	type = TEXT;
}

TextMessage::TextMessage(const std::string& text) : TextMessage(){
	this->text = text;
}

size_t TextMessage::pack(void** destination) const {
	uint8_t* buffer;
	size_t size = MessagePacket::pack(reinterpret_cast<void**>(&buffer));
	std::string text = this->text.substr(0, 40);

	size_t totalSize = size + 1 + text.size();
	buffer = static_cast<uint8_t*>(realloc(buffer, totalSize));

	*(buffer + size) = text.size();
	memcpy(buffer + size + 1, text.c_str(), text.size());

	*destination = buffer;
	return totalSize;
}

TextMessage* TextMessage::unpack(void* _buffer){
	uint8_t* buffer = static_cast<uint8_t*>(_buffer);
	TextMessage* message = new TextMessage;

	uint8_t size = *buffer;
	message->text = std::string(reinterpret_cast<char*>(buffer + 1), size);

	return message;
}

PicMessage::PicMessage(){
	type = PIC;
}

PicMessage::PicMessage(uint16_t index) : PicMessage(){
	this->index = index;
}

size_t PicMessage::pack(void** destination) const {
	uint8_t* buffer;
	size_t size = MessagePacket::pack(reinterpret_cast<void**>(&buffer));

	buffer = static_cast<uint8_t*>(realloc(buffer, size + 2));
	memcpy(buffer + size, &index, 2);

	*destination = buffer;
	return size + 2;
}

PicMessage* PicMessage::unpack(void* _buffer){
	PicMessage* message = new PicMessage;
	message->index = *static_cast<uint16_t*>(_buffer);
	return message;
}
