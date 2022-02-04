#include "SleepService.h"
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <Chatter.h>
#include "../LVScreen.h"
#include "LoRaService.h"
#include "../FSLVGL.h"
#include "../MainMenu.h"

SleepService Sleep;

RTC_DATA_ATTR bool SleepService::inDeepSleep = false;
RTC_DATA_ATTR uint16_t SleepService::deepSleepCount = 0;

SleepService::SleepService() : lightSleepTime(5), deepSleepTime(10), deepSleepWakeInterval(10), offTime(20){}

void SleepService::checkDeepSleep(){
	if(!inDeepSleep) return;

	inDeepSleep = false;
	state = DEEP;
}

void SleepService::begin(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);
	Messages.addReceivedListener(this);
	activityTime = millis();

	if(isDeepSleep()){
		exitDeepSleep();
	}
}

void SleepService::enterLightSleep(){
	if(state == LIGHT) return;
	state = LIGHT;

	Chatter.setBacklight(false);

	LVScreen* current = LVScreen::getCurrent();
	if(current){
		current->stop();
	}
}

void SleepService::exitLightSleep(){
	if(state != LIGHT) return;
	state = ON;

	Chatter.setBacklight(true);

	LVScreen* current = LVScreen::getCurrent();
	if(current){
		current->start();
	}
}

void SleepService::enterDeepSleep(){
	if(state == DEEP) return;
	state = DEEP;

	LoRa.radio.startReceive();
	inDeepSleep = true;
	ESP.deepSleep(deepSleepWakeInterval * 1000 * 1000);
}

void SleepService::exitDeepSleep(){
	if(state != DEEP) return;

	deepSleepCount++;
	deepSleepReceived = false;

	LoRa.initStateless();
	LoRa.loop();
	Messages.loop(0);
	LoRa.loop();

	state = ON;
	if(deepSleepReceived){
		deepSleepReceived = false;
		deepSleepCount = 0;

		FSLVGL::loadCache();
		auto screen = new MainMenu();
		screen->start();
		Chatter.setBacklight(true);
		activityTime = millis();
	}else{
		if(deepSleepCount * deepSleepWakeInterval >= offTime){
			turnOff();
		}

		enterDeepSleep();
	}
}

void SleepService::turnOff(){
	LoRa.radio.standby();
	LoRa.radio.sleep(false);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
	esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
	esp_deep_sleep_start();
}

void SleepService::loop(uint micros){
	uint32_t elapsed = floor(((float) millis() - (float) activityTime) / (1000.0f)); // in seconds

	if(elapsed >= lightSleepTime && state == ON){
		enterLightSleep();
	}else if(elapsed >= deepSleepTime && state == LIGHT){
		enterDeepSleep();
	}
}

void SleepService::anyKeyPressed(){
	activityTime = millis();

	if(state == LIGHT){
		exitLightSleep();
	}
}

void SleepService::msgReceived(const Message& message){
	if(state == LIGHT){
		activityTime = millis();
		exitLightSleep();
	}else if(state == DEEP){
		deepSleepReceived = true;
	}
}

bool SleepService::isDeepSleep(){
	return state == DEEP;
}
