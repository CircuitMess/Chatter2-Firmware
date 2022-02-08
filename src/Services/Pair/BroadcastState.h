#ifndef CHATTER_FIRMWARE_BROADCASTSTATE_H
#define CHATTER_FIRMWARE_BROADCASTSTATE_H
#include "State.h"

using namespace Pairing;

class BroadcastState : public State {
public:
	BroadcastState(PairService* pService);
	void loop(uint micros) override;
};


#endif //CHATTER_FIRMWARE_BROADCASTSTATE_H
