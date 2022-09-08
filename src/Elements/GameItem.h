#ifndef CHATTER_FIRMWARE_GAMEITEM_H
#define CHATTER_FIRMWARE_GAMEITEM_H

#include <Arduino.h>
#include <lvgl.h>
#include "../Interface/LVObject.h"
#include <string>

class GameItem : public LVObject {
public:
	GameItem(lv_obj_t* parent, const std::string& text, const std::string& icon);
	virtual ~GameItem();

protected:
	lv_style_t styleFocus;
	lv_style_t styleDef;
	lv_obj_t* name;
	lv_obj_t* image;
};


#endif //CHATTER_FIRMWARE_GAMEITEM_H
