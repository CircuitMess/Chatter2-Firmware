#ifndef CHATTER_FIRMWARE_SLEEPSERVICE_H
#define CHATTER_FIRMWARE_SLEEPSERVICE_H

#include <Arduino.h>
#include <Loop/LoopListener.h>
#include <Input/InputListener.h>
#include "MessageService.h"

class SleepService : public LoopListener, private InputListener {
public:
	SleepService();
	void begin();

	void loop(uint micros) override;

	void enterSleep();
	void turnOff();

	void resetActivity();
	void updateTimes();

private:
	uint32_t sleepTime; // [s]
	uint32_t shutdownTime; // [s]

	uint32_t activityTime;

	void anyKeyPressed() override;

};

extern SleepService Sleep;

#endif //CHATTER_FIRMWARE_SLEEPSERVICE_H
