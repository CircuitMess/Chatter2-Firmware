#ifndef CHATTER_FIRMWARE_BROADCASTSTATE_H
#define CHATTER_FIRMWARE_BROADCASTSTATE_H
#include "State.h"

using namespace Pairing;

class BroadcastState : public State {
public:
	BroadcastState(PairService* pService);
	void loop(uint micros) override;
private:
	const uint32_t bufferClearTime = 1000000;
	uint32_t buffTime = 0;
	bool buffCleared = false;
};


#endif //CHATTER_FIRMWARE_BROADCASTSTATE_H
