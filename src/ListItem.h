#ifndef CHATTER_FIRMWARE_LISTITEM_H
#define CHATTER_FIRMWARE_LISTITEM_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVObject.h"
#include <string>
#include <Input/InputListener.h>

class ListItem : public LVObject, private InputListener {
public:
	ListItem(lv_obj_t* parent, const std::string& text, uint8_t icon=0);

private:
	std::string text;
	uint8_t icon;
	uint8_t opa;
protected:
	lv_style_t styleFocus;
	lv_style_t styleDef;
	lv_obj_t* name;
};


#endif //CHATTER_FIRMWARE_LISTITEM_H
