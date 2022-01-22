#ifndef CHATTER_FIRMWARE_MESSAGESERVICE_H
#define CHATTER_FIRMWARE_MESSAGESERVICE_H

#include <Arduino.h>
#include "../Types.hpp"
#include "../Model/Message.h"
#include "../Model/Convo.hpp"
#include <string>
#include <Loop/LoopListener.h>

class MessageService : public LoopListener {
public:
	bool sendText(Convo& convo, const std::string& text);
	bool sendPic(Convo& convo, uint16_t index);

	void loop(uint micros) override;

private:
	bool sendMessage(Convo& convo, Message& message);

};


#endif //CHATTER_FIRMWARE_MESSAGESERVICE_H
