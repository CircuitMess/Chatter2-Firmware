#ifndef CHATTER_FIRMWARE_ACKNOWLEDGESTATE_H
#define CHATTER_FIRMWARE_ACKNOWLEDGESTATE_H

#include "State.h"
#include "../../Types.hpp"

using namespace Pairing;

class AcknowledgeState : public State {
public:
	AcknowledgeState(UID_t uid, uint8_t* key);
	void loop(uint micros) override;
private:
	UID_t uid;
	uint8_t key[32];

	uint32_t broadcastTime = 0;
	const uint32_t broadcastInterval = 1000000; //1s interval

	uint8_t ackCount = 0;
	uint8_t ackSuccessful = 0;
};


#endif //CHATTER_FIRMWARE_ACKNOWLEDGESTATE_H
