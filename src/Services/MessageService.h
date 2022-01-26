#ifndef CHATTER_FIRMWARE_MESSAGESERVICE_H
#define CHATTER_FIRMWARE_MESSAGESERVICE_H

#include <Arduino.h>
#include "../Types.hpp"
#include "../Model/Message.h"
#include "../Model/Convo.hpp"
#include "LoRaPacket.h"
#include <string>
#include <Loop/LoopListener.h>

class MessageService : public LoopListener {
public:
	bool sendText(Convo& convo, const std::string& text);
	bool sendPic(Convo& convo, uint16_t index);

	void begin();
	void loop(uint micros) override;

private:
	bool sendMessage(Convo& convo, Message& message);
	bool sendPacket(UID_t receiver, const Message& message);

	void receiveMessage(ReceivedPacket<MessagePacket>& packet);
	void receiveAck(ReceivedPacket<MessagePacket>& packet);

};

extern MessageService Messages;

#endif //CHATTER_FIRMWARE_MESSAGESERVICE_H
