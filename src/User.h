#ifndef CHATTER_FIRMWARE_USER_H
#define CHATTER_FIRMWARE_USER_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVObject.h"

class User : public LVObject {
public:
	User(lv_obj_t* parent, uint16_t hue, const String& name);

private:
	uint16_t hue;
	String name;

protected:
	lv_style_t styleFocus;
	lv_style_t styleDef;
};


#endif //CHATTER_FIRMWARE_USER_H
