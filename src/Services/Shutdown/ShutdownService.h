#ifndef CHATTER_FIRMWARE_SHUTDOWNSERVICE_H
#define CHATTER_FIRMWARE_SHUTDOWNSERVICE_H

#include <Loop/LoopListener.h>

class ShutdownService : public LoopListener{
public:
	void begin();
	void loop(uint micros) override;

private:
	bool warningShown = false;
	bool shutdownStarted = false;
	const uint32_t checkInterval = 60000000; //1 min
	uint32_t checkTimer = 0;

	void showWarning();
	void showShutdown();
};

extern ShutdownService Shutdown;

#endif //CHATTER_FIRMWARE_SHUTDOWNSERVICE_H
