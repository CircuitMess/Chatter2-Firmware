#ifndef CHATTER_FIRMWARE_STATE_H
#define CHATTER_FIRMWARE_STATE_H
#include <Arduino.h>
class PairService;

namespace Pairing {

class State {
public:
	State(PairService* pairService);
	virtual ~State();
	virtual void loop(uint micros) = 0;
protected:
	PairService* Pair;
};

}

#endif //CHATTER_FIRMWARE_STATE_H
