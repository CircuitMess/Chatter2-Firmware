#include "ShutdownService.h"
#include <Loop/LoopManager.h>
#include <Battery/BatteryService.h>

ShutdownService Shutdown;

void ShutdownService::begin(){
	checkTimer = checkInterval;
	LoopManager::addListener(this);

}

void ShutdownService::loop(uint micros){
	checkTimer+=micros;
	if(checkTimer >= checkInterval){
		checkTimer = 0;
		if(Battery.getPercentage() <= 1){
			showShutdown();
		}else if(Battery.getPercentage() <= 20 && !warningShown){
			warningShown = true;
			showWarning();
		}
	}
}

void ShutdownService::showWarning(){

}

void ShutdownService::showShutdown(){

}
