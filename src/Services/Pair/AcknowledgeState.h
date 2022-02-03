#ifndef CHATTER_FIRMWARE_ACKNOWLEDGESTATE_H
#define CHATTER_FIRMWARE_ACKNOWLEDGESTATE_H

#include "State.h"
#include "../../Types.hpp"

using namespace Pairing;

class AcknowledgeState : public State {
public:
	AcknowledgeState(UID_t uid, uint8_t* key, PairService* pairService);
	void loop(uint micros) override;
private:
	UID_t uid;
	uint8_t key[32];

	uint32_t broadcastTime = broadcastInterval;
	const uint32_t broadcastInterval = 2000000; //1s interval

	uint8_t ackSent = 0;
	uint8_t ackSuccessful = 0;
};


#endif //CHATTER_FIRMWARE_ACKNOWLEDGESTATE_H
