#ifndef CHATTER_FIRMWARE_SLEEPSERVICE_H
#define CHATTER_FIRMWARE_SLEEPSERVICE_H

#include <Arduino.h>
#include <Loop/LoopListener.h>
#include <Input/InputListener.h>
#include "MessageService.h"

class SleepService : public LoopListener, private InputListener, private MsgReceivedListener {
public:
	SleepService();
	void begin();

	void loop(uint micros) override;

	void enterLightSleep();
	void exitLightSleep();
	void enterDeepSleep();
	void exitDeepSleep();
	void turnOff();

	void resetActivity();
	void updateTimes();

private:
	enum State {
		ON, LIGHT, DEEP, OFF
	} state = ON;

	uint32_t lightSleepTime; // [s]
	uint32_t deepSleepTime; // [s]
	uint32_t offTime; // [s]

	bool deepSleepReceived = false;
	uint32_t deepSleepTotal = 0; // [ms]

	uint32_t activityTime;

	void anyKeyPressed() override;
	void msgReceived(const Message& message) override;

};

extern SleepService Sleep;

#endif //CHATTER_FIRMWARE_SLEEPSERVICE_H
