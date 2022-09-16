#include "SleepService.h"
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <Chatter2.h>
#include "../Interface/LVScreen.h"
#include "../Interface/LVModal.h"
#include "LoRaService.h"
#include "../Games/GameEngine/Game.h"
#include "../FSLVGL.h"
#include "../Screens/LockScreen.h"
#include <Settings.h>
#include <driver/rtc_io.h>

SleepService Sleep;

extern bool gameStarted;
extern Game* startedGame;

SleepService::SleepService(){

}

void SleepService::begin(){
	updateTimes();

	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);

	resetActivity();
}

void SleepService::updateTimes(){
	sleepTime = SleepSeconds[Settings.get().sleepTime];
	shutdownTime = ShutdownSeconds[Settings.get().shutdownTime];
}

void SleepService::resetActivity(){
	activityTime = millis();
}

void SleepService::loop(uint micros){
	uint32_t elapsed = floor(((float) millis() - (float) activityTime) / (1000.0f)); // in seconds

	if(sleepTime && elapsed >= sleepTime){
		enterSleep();
	}
}

void SleepService::anyKeyPressed(){
	resetActivity();
}

void SleepService::enterSleep(){
	if(Chatter.backlightPowered()){
		Chatter.fadeOut();
	}

	LVScreen* screen;
	if(gameStarted){
		screen = startedGame->getGamesScreen();
		startedGame->stop();
		delete startedGame;
		gameStarted = false;
		startedGame = nullptr;
		FSLVGL::loadCache();
	}else{
		screen = LVScreen::getCurrent();
		screen->stop();
	}

	while(LoRa.working){
		delay(1);
	}

	// LoRa wakeup
	LoRa.radio.setDio1Action(LoRaService::moduleInterrupt);
	LoRa.radio.startReceive();
	esp_sleep_enable_gpio_wakeup();
	gpio_wakeup_enable((gpio_num_t) RADIO_DIO1, GPIO_INTR_HIGH_LEVEL);

	// GPIO wakeup
	esp_sleep_enable_ext1_wakeup((uint64_t) 1 << PIN_WAKE, ESP_EXT1_WAKEUP_ALL_LOW);
	//gpio_wakeup_enable((gpio_num_t) PIN_WAKE, GPIO_INTR_LOW_LEVEL);

	// Timer wakeup
	// esp_sleep_enable_timer_wakeup(60*1000*1000); // TODO: turn off time

	// Sleep
	esp_light_sleep_start();

	auto cause = esp_sleep_get_wakeup_cause();
	printf("Wakeup: %d\n", cause);

	rtc_gpio_deinit((gpio_num_t) PIN_WAKE);

	do {
		delay(10);
	}while(LoRa.working);
	for(int i = 0; i < 4; i++){
		Messages.loop(0);
	}

	LoopManager::resetTime();
	resetActivity();
	lv_timer_handler();

	LoopManager::defer([screen](uint32_t dt){
		LockScreen::activate(screen);
		lv_timer_handler();

		Chatter.fadeIn();
	});

	printf("Exited light sleep\n");
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
