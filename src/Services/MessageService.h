#ifndef CHATTER_FIRMWARE_MESSAGESERVICE_H
#define CHATTER_FIRMWARE_MESSAGESERVICE_H

#include <Arduino.h>
#include "../Types.hpp"
#include "../Model/Message.h"
#include "../Model/Convo.hpp"
#include "LoRaPacket.h"
#include <string>
#include <Loop/LoopListener.h>
#include <unordered_map>
#include "../WithListeners.h"

class MsgReceivedListener;
class MsgChangedListener;

class MessageService : public LoopListener, public WithListeners<MsgReceivedListener>, public WithListeners<MsgChangedListener> {
public:
	Message sendText(UID_t convo, const std::string& text);
	Message sendPic(UID_t convo, uint16_t index);

	Message resend(UID_t convo, UID_t message);

	bool deleteMessage(UID_t convo, UID_t msg);

	Message getLastMessage(UID_t convo);

	void begin();
	void loop(uint micros) override;

	void addReceivedListener(MsgReceivedListener* listener);
	void addChangedListener(MsgChangedListener* listener);

	void removeReceivedListener(MsgReceivedListener* listener);
	void removeChangedListener(MsgChangedListener* listener);

private:
	Message sendMessage(UID_t convo, Message& message);
	bool sendPacket(UID_t receiver, const Message& message);

	void receiveMessage(ReceivedPacket<MessagePacket>& packet);
	void receiveAck(ReceivedPacket<MessagePacket>& packet);

	std::unordered_map<UID_t, Message> lastMessages;

};

class MsgReceivedListener {
friend MessageService;
private:
	virtual void msgReceived(const Message& message) = 0;
};

class MsgChangedListener {
friend MessageService;
private:
	virtual void msgChanged(const Message& message) = 0;
};

extern MessageService Messages;

#endif //CHATTER_FIRMWARE_MESSAGESERVICE_H
