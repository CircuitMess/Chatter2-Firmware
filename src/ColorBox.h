#ifndef CHATTER_FIRMWARE_COLORBOX_H
#define CHATTER_FIRMWARE_COLORBOX_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVObject.h"
#include <Input/InputListener.h>

class ColorBox : public InputListener{
public:
	ColorBox(lv_obj_t *parent,uint16_t hue);
	virtual ~ColorBox();
	lv_obj_t* getLvObj();
	void setColor(uint16_t hue);

protected:
	lv_style_t styleFocus;
	lv_style_t styleDef;
	uint16_t hue;
	lv_obj_t* slider;

private:
	lv_style_t style_colorPicker;
	lv_style_t style_knob;
	lv_style_t style_def;
	lv_style_t style_focused;

	bool heldThresh = false;
	void buttonHeldRepeat(uint i, uint repeatCount) override;
	void buttonHeld(uint i) override;
	void buttonReleased(uint i) override;
};

#endif //CHATTER_FIRMWARE_COLORBOX_H
