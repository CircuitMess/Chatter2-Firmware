#ifndef CHATTER_FIRMWARE_COLORBOXLVGL_H
#define CHATTER_FIRMWARE_COLORBOXLVGL_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVObject.h"

class ColorBox : public LVObject{
public:
	ColorBox(lv_obj_t *parent,lv_color_t color);

private:
	lv_color_t color;
protected:
	lv_style_t styleFocus;
	lv_style_t styleDef;
};

#endif //CHATTER_FIRMWARE_COLORBOXLVGL_H
