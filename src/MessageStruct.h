#ifndef CHATTER_FIRMWARE_MESSAGESTRUCT_H
#define CHATTER_FIRMWARE_MESSAGESTRUCT_H

#include "types.hpp"

struct MessageStruct {
	UID_t sender;
	bool received;
	enum class MessageType {
		TEXT, PIC
	} type;
	UID_t uid;
	char* content;

};
#endif //CHATTER_FIRMWARE_MESSAGESTRUCT_H
