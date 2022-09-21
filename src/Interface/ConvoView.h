#ifndef CHATTER_FIRMWARE_CONVOVIEW_H
#define CHATTER_FIRMWARE_CONVOVIEW_H

#include "../Model/Convo.hpp"
#include "../Model/Message.h"
#include "../Storage/Storage.h"
#include "../Services/MessageService.h"

struct ConversationStruct;

class ConvoView : private MsgChangedListener {
public:
	static const uint8_t Count = 16;
	const UID_t convoUID;

	ConvoView(UID_t convo);
	virtual ~ConvoView();

	void load(size_t startIndex);
	void loadLatest();

	void listen();

	const std::vector<Message>& getMessages() const;
	size_t getStartIndex() const;
	size_t getTotalMessageCount() const;
	bool isLatest() const;

	void addMessage(const Message& message);

private:
	std::vector<Message> messages;
	size_t startIndex;
	size_t totalMessageCount;
	bool latest;

	void msgChanged(const Message& message) override;

};


#endif //CHATTER_FIRMWARE_CONVOVIEW_H
