#ifndef CHATTER_FIRMWARE_LORAPACKET_H
#define CHATTER_FIRMWARE_LORAPACKET_H

#include <Arduino.h>
#include <string>
#include "../Types.hpp"
#include "../Model/Profile.hpp"

struct LoRaPacket {
	const uint8_t header[8] = {};
	uint32_t checksum;

	UID_t sender;
	UID_t receiver;
	hash_t profileHash;

	enum Type : uint8_t {
		MSG, PROF, PAIR_REQ, PAIR_BROADCAST, PAIR_ACK
	} type;

	size_t size;
	void* content;
};

template<typename T>
struct ReceivedPacket {
	UID_t sender;
	T* content;
};

struct Packet {
	virtual size_t pack(void** destination) const = 0;
};

struct MessagePacket : Packet {
	enum Type : uint8_t {
		TEXT, PIC, ACK
	} type;
	UID_t uid;

	virtual size_t pack(void** destination) const override;
	static MessagePacket* unpack(void* buffer);
};

struct TextMessage : MessagePacket {
	TextMessage();
	TextMessage(const std::string& text);

	std::string text;

	virtual size_t pack(void** destination) const override;
	static TextMessage* unpack(void* buffer);
};

struct PicMessage : MessagePacket {
	PicMessage();
	PicMessage(uint8_t index);

	uint8_t index;

	size_t pack(void** destination) const override;
	static PicMessage* unpack(void* buffer);
};

struct ProfilePacket : Packet{
	enum Type : uint8_t{
		REQ, RESP
	} type;

	size_t pack(void** destination) const override;
	static ProfilePacket* unpack(void* buffer);

};

struct ProfileResponse : ProfilePacket{
	ProfileResponse();
	ProfileResponse(const Profile& prof);

	Profile profile;

	size_t pack(void** destination) const override;
	static ProfileResponse* unpack(void* buffer);
};

struct AdvertisePair : Packet{
	AdvertisePair() = default;
	AdvertisePair(const Profile& prof);

	Profile profile;

	size_t pack(void** destination) const override;
	static AdvertisePair* unpack(void* buffer);
};

struct RequestPair : Packet{
	RequestPair() = default;

	uint8_t encKey[32];

	size_t pack(void** destination) const override;
	static RequestPair* unpack(void* buffer);
};

struct AckPair : Packet{
	AckPair() = default;

	uint8_t encKey[32];

	size_t pack(void** destination) const override;
	static AckPair* unpack(void* buffer);
};

#endif //CHATTER_FIRMWARE_LORAPACKET_H
