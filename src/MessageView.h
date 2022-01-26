#ifndef CHATTER_FIRMWARE_MESSAGEVIEW_H
#define CHATTER_FIRMWARE_MESSAGEVIEW_H

#include "MessageStruct.h"
#include "Model/Convo.hpp"
#include "Model/Message.h"
#include "Storage/Storage.h"

struct ConversationStruct;

class MessageView {
public:
	static const uint8_t Count = 10;
	const UID_t convoUID;

	MessageView(UID_t convo);
	void load(size_t startIndex);
	void loadLatest();

	const std::vector<Message>& getMessages() const;
	size_t getStartIndex() const;
	size_t getTotalMessageCount() const;

private:
	std::vector<Message> messages;
	size_t startIndex;
	size_t totalMessageCount;

};


#endif //CHATTER_FIRMWARE_MESSAGEVIEW_H
