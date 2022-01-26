#include <Arduino.h>
#include <CircuitOS.h>
#include <Chatter.h>
#include <Loop/LoopManager.h>
#include <lvgl.h>
#include "src/InputChatter.h"
#include "src/FSLVGL.h"
#include "src/ChatterTheme.h"
#include <SPIFFS.h>
#include "src/MainMenu.h"
#include "src/Storage/Storage.h"
#include "src/Services/LoRaService.h"

lv_disp_draw_buf_t drawBuffer;
Display* display;

void lvglFlush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p){
	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);

	TFT_eSPI &tft = *display->getTft();
	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors(&color_p->full, w * h, true);
	tft.endWrite();
	lv_disp_flush_ready(disp);
}

struct {
	UID_t uid;
	const char* nickname;
	uint8_t avatar;
	uint16_t hue;
} const Chatters[] = {
		{ 0xdcba4bf23a08, "Filip v0.5", 1, 0 },
		{ 0x48275612cfa4, "Filip v0.6", 2, 80 },
		{ 0x48e4e0e6e2e0, "Emil v0.4", 3, 160 },
		{ 0xe4c14bf23a08, "Hrvoje v0.5", 4, 240 },
};

void loadMock(bool clear = false){
	if(clear){
		for(UID_t uid : Storage.Messages.all()) Storage.Messages.remove(uid);
		for(UID_t uid : Storage.Friends.all()) Storage.Friends.remove(uid);
		for(UID_t uid : Storage.Convos.all()) Storage.Convos.remove(uid);
	}

	for(const auto& chatter : Chatters){
		if(chatter.uid == ESP.getEfuseMac()) continue;

		Friend fren;
		fren.uid = chatter.uid;
		fren.profile.avatar = chatter.avatar;
		fren.profile.color = (uint8_t) (chatter.hue / 2);
		strncpy(fren.profile.nickname, chatter.nickname, 15);
		Storage.Friends.add(fren);

		Convo convo;
		convo.uid = fren.uid;

		Message message("Hello how are you!");
		message.uid = LoRa.randUID();
		convo.messages.push_back(message.uid);

		Storage.Messages.add(message);
		Storage.Convos.add(convo);
	}
}

void printData(){
	for(UID_t uid : Storage.Friends.all()){
		Friend fren = Storage.Friends.get(uid);
		printf("%llx | Fren: %s | Hue: %d | Avatar: %d\n", fren.uid, fren.profile.nickname, fren.profile.color, fren.profile.avatar);

		Convo convo = Storage.Convos.get(uid);
		printf("%llx | Convo: %d messages\n", convo.uid, convo.messages.size());

		for(UID_t uid : convo.messages){
			Message message = Storage.Messages.get(uid);
			printf("%llx | Message: %s\n", message.uid, message.getText().c_str());
		}
	}
}

void setup(){
	Serial.begin(115200);
	Chatter.begin();
	display = Chatter.getDisplay();

	lv_init();
	lv_disp_draw_buf_init(&drawBuffer, display->getBaseSprite()->getBuffer(), NULL, 160 * 128);

	static lv_disp_drv_t displayDriver;
	lv_disp_drv_init(&displayDriver);
	displayDriver.hor_res = 160;
	displayDriver.ver_res = 128;
	displayDriver.flush_cb = lvglFlush;
	displayDriver.draw_buf = &drawBuffer;
	lv_disp_t * disp = lv_disp_drv_register(&displayDriver);
	chatterThemeInit(disp);

	new FSLVGL(SPIFFS, 'S');

	Chatter.getInput()->addListener(new InputChatter());

	LoRa.begin();

	loadMock(true);
	printData();

	auto screen = new MainMenu();
	screen->start();
}

void loop(){
	lv_timer_handler();
	LoopManager::loop();
}

