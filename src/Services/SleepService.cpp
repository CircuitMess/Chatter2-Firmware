#include "SleepService.h"
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <Chatter.h>
#include "../Interface/LVScreen.h"
#include "../Interface/LVModal.h"
#include "LoRaService.h"
#include <Settings.h>

SleepService Sleep;

SleepService::SleepService() : offTime(60 * 60){

}


void SleepService::begin(){
	updateTimes();

	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);
	Messages.addReceivedListener(this);

	resetActivity();
}

void SleepService::updateTimes(){
	lightSleepTime = SleepSeconds[Settings.get().sleepTime];
	deepSleepTime = ShutdownSeconds[Settings.get().shutdownTime];
}

void SleepService::enterLightSleep(){
	if(state == LIGHT) return;
	state = LIGHT;

	if(Chatter.backlightPowered()){
		Chatter.fadeOut();
	}

	LVScreen* current = LVScreen::getCurrent();
	if(current){
		current->stop();
	}
}

void SleepService::exitLightSleep(){
	if(state != LIGHT) return;
	state = ON;

	LVScreen* current = LVScreen::getCurrent();
	LVModal* currentModal = LVModal::getCurrent();
	if(current){
		current->start();
		if(currentModal){
			currentModal->start();
		}
		lv_timer_handler();
	}

	Chatter.fadeIn();
}

void SleepService::enterDeepSleep(){
	if(state == DEEP) return;

	if(state == ON){
		enterLightSleep();
	}

	state = DEEP;

	while(LoRa.working){
		delay(1);
	}

	uint32_t time = millis();

	LoRa.radio.setDio1Action(LoRaService::moduleInterrupt);
	LoRa.radio.startReceive();

	esp_sleep_enable_gpio_wakeup();
	gpio_wakeup_enable((gpio_num_t) RADIO_DIO1, GPIO_INTR_HIGH_LEVEL);
	esp_sleep_enable_timer_wakeup(60*1000*1000);
	esp_light_sleep_start();

	deepSleepTotal += millis() - time;

	exitDeepSleep();
}

void SleepService::exitDeepSleep(){
	if(state != DEEP) return;

	deepSleepReceived = false;
	uint32_t time = millis();

	// Wait for LoRa to process packets
	delay(2000);
	while(LoRa.working){
		delay(500);
	}

	// Trigger MessageService packet processing
	for(int i = 0; i < 6; i++){
		Messages.loop(0);
	}

	enterLightSleep();
	if(deepSleepReceived){
		deepSleepReceived = false;
		deepSleepTotal = 0;
		resetActivity();

		exitLightSleep();
	}else{
		deepSleepTotal += millis() - time;

		if(deepSleepTotal >= offTime * 1000){
			turnOff();
			return;
		}

		enterDeepSleep();
	}
}

void SleepService::turnOff(){
	ledcDetachPin(PIN_BUZZ);
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

	if(lightSleepTime && elapsed >= lightSleepTime && state == ON){
		enterLightSleep();
	}else if(deepSleepTime && elapsed >= deepSleepTime && (state == LIGHT || state == ON)){
		enterDeepSleep();
	}
}

void SleepService::anyKeyPressed(){
	resetActivity();

	if(state == LIGHT){
		exitLightSleep();
	}
}

void SleepService::msgReceived(const Message& message){
	if(state == LIGHT){
		resetActivity();
		exitLightSleep();
	}else if(state == DEEP){
		deepSleepReceived = true;
	}
}

void SleepService::resetActivity(){
	activityTime = millis();
}
