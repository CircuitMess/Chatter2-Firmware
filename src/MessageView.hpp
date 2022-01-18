#ifndef CHATTER_FIRMWARE_MESSAGEVIEW_HPP
#define CHATTER_FIRMWARE_MESSAGEVIEW_HPP

#include "MessageStruct.h"

struct ConversationStruct;

struct MessageView {
	uint count;
	MessageStruct* messages = nullptr;
	uint startIndex;
	ConversationStruct* convo;

	void update(uint startIndex, uint count){

		this->startIndex = startIndex;
		this->count = count;
		free(messages);
		messages = (MessageStruct*)malloc(sizeof(MessageStruct) * count);
		//TODO - uzimanje podataka od repoa
		for(int i = 0; i < count; ++i){
			char* newString = (char*)malloc(3);
			sprintf(newString, "%d", startIndex + i);
			messages[i] = MessageStruct{1, ((random(0, 3) % 2) == 0), MessageStruct::MessageType::TEXT, 0, newString};
		}
	}

	MessageView(uint count, uint startIndex, ConversationStruct* convo) : count(count), startIndex(startIndex), convo(convo){
		update(startIndex, count);
	}
};


#endif //CHATTER_FIRMWARE_MESSAGEVIEW_HPP
