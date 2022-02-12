#ifndef CHATTER_FIRMWARE_LISTITEM_H
#define CHATTER_FIRMWARE_LISTITEM_H

#include <Arduino.h>
#include <lvgl.h>
#include "../Interface/LVObject.h"
#include <string>

class ListItem : public LVObject {
public:
	ListItem(lv_obj_t* parent, const std::string& text, uint8_t icon=0);
	virtual ~ListItem();

protected:
	lv_style_t styleFocus;
	lv_style_t styleDef;
	lv_obj_t* name;
};


#endif //CHATTER_FIRMWARE_LISTITEM_H
