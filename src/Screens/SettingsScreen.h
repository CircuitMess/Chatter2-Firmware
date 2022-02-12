#ifndef CHATTER_FIRMWARE_SETTINGSSCREEN_H
#define CHATTER_FIRMWARE_SETTINGSSCREEN_H

#include "../Interface/LVScreen.h"
#include <Input/InputListener.h>

class SettingsScreen : public LVScreen, private InputListener {
public:
	SettingsScreen();

	virtual ~SettingsScreen();

	void onStarting() override;

	void onStop() override;

private:
	lv_obj_t* sound;
	lv_obj_t* sleepTime;
	lv_obj_t* shutdownTime;
	lv_obj_t* screenBrightness;
	lv_obj_t* factoryReset;
	lv_obj_t* HWTest;
	lv_obj_t* version;

	lv_obj_t* soundSwitch;
	lv_obj_t* sleepTimeLabel;
	lv_obj_t* shutdownTimeLabel;
	lv_obj_t* sleepSlider;
	lv_obj_t* shutdownSlider;
	lv_obj_t* brightnessSlider;

	lv_style_t style_knob;
	lv_style_t style_def;
	lv_style_t style_focused;
	lv_style_t style_main;
	lv_style_t style_pressed;

	bool heldThresh = false;
	void buttonHeldRepeat(uint i, uint repeatCount) override;
	void buttonHeld(uint i) override;
	void buttonReleased(uint i) override;

};

#endif //CHATTER_FIRMWARE_SETTINGSSCREEN_H
