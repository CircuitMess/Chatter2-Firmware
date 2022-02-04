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

	void checkDeepSleep();
	bool isDeepSleep();

private:
	enum State {
		ON, LIGHT, DEEP, OFF
	} state = ON;

	const uint32_t lightSleepTime;
	const uint32_t deepSleepTime;
	const uint32_t deepSleepWakeInterval;
	const uint32_t offTime;

	static bool inDeepSleep;
	bool deepSleepReceived = false;

	static uint16_t deepSleepCount;

	uint32_t activityTime;

	void anyKeyPressed() override;
	void msgReceived(const Message& message) override;

};

extern SleepService Sleep;

#endif //CHATTER_FIRMWARE_SLEEPSERVICE_H
