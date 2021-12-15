#ifndef CHATTER_FIRMWARE_TEXTENTRY_H
#define CHATTER_FIRMWARE_TEXTENTRY_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVObject.h"
#include <string>
#include <Input/InputListener.h>

class TextEntry : public LVObject, private InputListener{
public:
	TextEntry(lv_obj_t* parent, std::string text = "");

private:
	std::string text;

protected:
	lv_style_t styleFocus;
	lv_style_t styleDef;
};

#endif //CHATTER_FIRMWARE_TEXTENTRY_H
