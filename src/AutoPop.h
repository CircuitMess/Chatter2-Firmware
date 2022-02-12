#ifndef CHATTER_FIRMWARE_AUTOPOP_H
#define CHATTER_FIRMWARE_AUTOPOP_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "Interface/LVScreen.h"

class AutoPop : private InputListener {
public:
	AutoPop(LVScreen* screen);
	virtual ~AutoPop();

	void start();
	void stop();

private:
	LVScreen* screen;

	void buttonPressed(uint i) override;

};


#endif //CHATTER_FIRMWARE_AUTOPOP_H
