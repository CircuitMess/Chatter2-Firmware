#ifndef CHATTER_FIRMWARE_STATE_H
#define CHATTER_FIRMWARE_STATE_H
#include <Arduino.h>
namespace Pairing {

class State {
public:
	virtual ~State();
	virtual void loop(uint micros) = 0;
};

}

#endif //CHATTER_FIRMWARE_STATE_H
