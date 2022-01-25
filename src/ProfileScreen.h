#ifndef CHATTER_FIRMWARE_PROFILESCREEN_H
#define CHATTER_FIRMWARE_PROFILESCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVScreen.h"

class ProfileScreen : public LVScreen{
public:
	ProfileScreen();
private:
	lv_obj_t* name;
	lv_style_t textStyle;

	void buildHeader();
	void buildBody();
	void buildFooter();

	void exitTextEntry();
};

#endif //CHATTER_FIRMWARE_PROFILESCREEN_H