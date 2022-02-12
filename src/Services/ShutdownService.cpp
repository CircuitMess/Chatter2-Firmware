#include "ShutdownService.h"
#include <Loop/LoopManager.h>
#include <Battery/BatteryService.h>
#include "../Modals/BatteryNotification.h"
#include "SleepService.h"

ShutdownService Shutdown;

void ShutdownService::begin(){
	checkTimer = checkInterval;
	LoopManager::addListener(this);
}

void ShutdownService::loop(uint micros){
	checkTimer+=micros;
	if(checkTimer >= checkInterval){
		checkTimer = 0;
		if(Battery.getPercentage() <= 1 && !shutdownStarted){
			shutdownStarted = true;
			showShutdown();
		}else if(Battery.getPercentage() <= 10 && !warningShown){
			warningShown = true;
			showWarning();
		}
	}
}

void ShutdownService::showWarning(){
	Sleep.resetActivity();
	(new BatteryNotification(LVScreen::getCurrent(), BatteryNotification::WARNING))->start();
}

void ShutdownService::showShutdown(){
	Sleep.resetActivity();
	(new BatteryNotification(LVScreen::getCurrent(), BatteryNotification::SHUTDOWN))->start();
}
