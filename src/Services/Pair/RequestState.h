#ifndef CHATTER_FIRMWARE_REQUESTSTATE_H
#define CHATTER_FIRMWARE_REQUESTSTATE_H

#include "State.h"
#include "../../Types.hpp"

using namespace Pairing;

class RequestState : public State {
public:
	RequestState(UID_t uid, PairService* pairService);
	void loop(uint micros) override;
private:
	UID_t uid;
	uint8_t key[32];

	uint32_t broadcastTime = broadcastInterval;
	const uint32_t broadcastInterval = 1000000; //1s interval
};


#endif //CHATTER_FIRMWARE_REQUESTSTATE_H
