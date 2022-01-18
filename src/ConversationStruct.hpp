#ifndef CHATTER_FIRMWARE_CONVERSATIONSTRUCT_HPP
#define CHATTER_FIRMWARE_CONVERSATIONSTRUCT_HPP

#include "MessageView.hpp"
#include "Friend.hpp"

struct ConversationStruct {
	Friend* person;
	uint messageCount;
	UID_t uid;

	MessageView getMessageView(uint startIndex, uint count){

		return {count, startIndex, this};
	}
};

#endif //CHATTER_FIRMWARE_CONVERSATIONSTRUCT_HPP
