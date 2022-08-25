#include "ShutdownService.h"
#include <Loop/LoopManager.h>
#include <Battery/BatteryService.h>
#include "../Modals/BatteryNotification.h"
#include "SleepService.h"
#include "../Games/GameEngine/Game.h"

ShutdownService Shutdown;

void ShutdownService::begin(){
	checkTimer = checkInterval;
	LoopManager::addListener(this);
}

void ShutdownService::loop(uint micros){
	extern bool gameStarted;
	if(gameStarted) showShutdown();

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
	extern bool gameStarted;
	if(gameStarted) return;

	Sleep.resetActivity();
	(new BatteryNotification(LVScreen::getCurrent(), BatteryNotification::WARNING))->start();
}

void ShutdownService::showShutdown(){
	extern bool gameStarted;
	if(gameStarted){
		extern Game* startedGame;
		if(startedGame){
			startedGame->pop();
		}
	}
	Sleep.resetActivity();
	(new BatteryNotification(LVScreen::getCurrent(), BatteryNotification::SHUTDOWN))->start();
}
