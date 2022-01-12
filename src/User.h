#ifndef CHATTER_FIRMWARE_USER_H
#define CHATTER_FIRMWARE_USER_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVObject.h"
#include "ProfileStruct.hpp"
class User : public LVObject {
public:
	User(lv_obj_t* parent, const ProfileStruct &profile);

protected:
	lv_style_t styleFocus;
	lv_style_t styleDef;

	lv_obj_t* avatar;
	lv_obj_t* name;
};


#endif //CHATTER_FIRMWARE_USER_H
