#ifndef CHATTER_FIRMWARE_SETTINGSSCREEN_H
#define CHATTER_FIRMWARE_SETTINGSSCREEN_H

#include "../LVScreen.h"

class SettingsScreen : public LVScreen {
public:
	SettingsScreen();

	virtual ~SettingsScreen();

	void onStarting() override;

	void onStop() override;

private:
	lv_obj_t* sound;
	lv_obj_t* sleepTime;
	lv_obj_t* screenBrightness;
	lv_obj_t* factoryReset;
	lv_obj_t* HWTest;
	lv_obj_t* version;

	lv_obj_t* soundSwitch;
	lv_obj_t* sleepTimeLabel;
	lv_obj_t* sleepSlider;
	lv_obj_t* brightnessSlider;

	lv_style_t style_knob;
	lv_style_t style_def;
	lv_style_t style_focused;
	lv_style_t style_main;
	lv_style_t style_pressed;

	char sleepBuf[5];

};

#endif //CHATTER_FIRMWARE_SETTINGSSCREEN_H
