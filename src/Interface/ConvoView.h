#ifndef CHATTER_FIRMWARE_CONVOVIEW_H
#define CHATTER_FIRMWARE_CONVOVIEW_H

#include "../Model/Convo.hpp"
#include "../Model/Message.h"
#include "../Storage/Storage.h"

struct ConversationStruct;

class ConvoView {
public:
	static const uint8_t Count = 16;
	const UID_t convoUID;

	ConvoView(UID_t convo);
	void load(size_t startIndex);
	void loadLatest();

	const std::vector<Message>& getMessages() const;
	size_t getStartIndex() const;
	size_t getTotalMessageCount() const;
	bool isLatest() const;

private:
	std::vector<Message> messages;
	size_t startIndex;
	size_t totalMessageCount;
	bool latest;

};


#endif //CHATTER_FIRMWARE_CONVOVIEW_H
