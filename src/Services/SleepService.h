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

	void enterSleep();
	void turnOff();

	void resetActivity();
	void updateTimes();

	virtual void msgReceived(const Message& message);

private:
	uint32_t sleepTime; // [s]
	uint32_t shutdownTime; // [s]

	uint32_t activityTime;

	void anyKeyPressed() override;

	bool gotMessage = false;

};

extern SleepService Sleep;

#endif //CHATTER_FIRMWARE_SLEEPSERVICE_H
