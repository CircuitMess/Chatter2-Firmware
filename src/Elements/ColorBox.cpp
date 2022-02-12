#include <Pins.hpp>
#include "ColorBox.h"
#include <Input/Input.h>

ColorBox::ColorBox(lv_obj_t* parent, uint16_t hue){

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;

	setButtonHoldAndRepeatTime(BTN_LEFT, 50);
	setButtonHoldAndRepeatTime(BTN_RIGHT, 50);
	setButtonHoldTime(BTN_LEFT, 400);
	setButtonHoldTime(BTN_RIGHT, 400);

	slider = lv_slider_create(parent);
	lv_slider_set_range(slider, 0, 45);    // 360 = 45 * 8
	lv_slider_set_value(slider, hue / 8, LV_ANIM_OFF);
	lv_obj_remove_style_all(slider);        /*Remove the styles coming from the theme*/
	lv_obj_set_size(slider, 45, 20);

	//knob
	lv_style_init(&style_knob);
	lv_style_set_bg_opa(&style_knob, LV_OPA_0);
	lv_style_set_bg_img_src(&style_knob, LV_SYMBOL_DOWN);
	lv_style_set_pad_top(&style_knob, 13);
	lv_style_set_text_font(&style_knob, &lv_font_montserrat_8);
	lv_style_set_text_color(&style_knob, lv_color_white());
	lv_obj_add_style(slider, &style_knob, LV_PART_KNOB | LV_STATE_EDITED);

	//edited state
	lv_style_init(&style_colorPicker);
	lv_style_set_bg_opa(&style_colorPicker, LV_OPA_0);
	lv_style_set_bg_img_src(&style_colorPicker, "S:/colorPicker.bin");
	lv_style_set_border_width(&style_colorPicker, 0);
	lv_obj_add_style(slider, &style_colorPicker, LV_PART_MAIN | LV_STATE_EDITED);

	//default state
	lv_style_init(&style_def);
	lv_style_set_bg_opa(&style_def, LV_OPA_100);
	lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_RED));
	lv_style_set_pad_all(&style_def, 1);
	lv_style_set_border_width(&style_def, 0);
	lv_style_set_height(&style_def, 20);
	lv_obj_add_style(slider, &style_def, sel);

	//focused state
	lv_style_init(&style_focused);
	lv_style_set_border_width(&style_focused, 1);
	lv_style_set_border_color(&style_focused, lv_color_white());
	lv_style_set_border_opa(&style_focused, LV_OPA_100);
	lv_style_set_pad_all(&style_focused, 0);
	lv_obj_add_style(slider, &style_focused, selFocus);

	setColor(hue);

	lv_obj_add_event_cb(slider, [](lv_event_t* event){
		Input::getInstance()->addListener(static_cast<ColorBox*>(lv_event_get_user_data(event)));
	}, LV_EVENT_FOCUSED, this);

	lv_obj_add_event_cb(slider, [](lv_event_t* event){
		Input::getInstance()->removeListener(static_cast<ColorBox*>(lv_event_get_user_data(event)));
	}, LV_EVENT_DEFOCUSED, this);

	lv_obj_add_event_cb(slider, [](lv_event_t* event){
		if(lv_event_get_key(event) == LV_KEY_ESC){
			lv_group_set_editing((lv_group_t*)lv_obj_get_group(lv_event_get_target(event)), false);
		}
	}, LV_EVENT_KEY, this);

	lv_obj_add_event_cb(slider, [](lv_event_t* event){
		static_cast<ColorBox*>(lv_event_get_user_data(event))->setColor(lv_slider_get_value(lv_event_get_target(event)) * 8);
	}, LV_EVENT_VALUE_CHANGED, this);

	lv_obj_add_event_cb(slider, [](lv_event_t* event){
		delete static_cast<ColorBox*>(lv_event_get_user_data(event));
	}, LV_EVENT_DELETE, this);
}

ColorBox::~ColorBox(){
	Input::getInstance()->removeListener(this);
	lv_style_reset(&style_colorPicker);
	lv_style_reset(&style_knob);
	lv_style_reset(&style_def);
	lv_style_reset(&style_focused);
}

lv_obj_t* ColorBox::getLvObj(){
	return slider;
}

void ColorBox::setColor(uint16_t hue){
	this->hue = hue;
	lv_style_set_bg_color(&style_def, lv_color_hsv_to_rgb(hue, 100, 100));
}

void ColorBox::buttonHeldRepeat(uint i, uint repeatCount){
	if(!lv_obj_has_state(slider, LV_STATE_EDITED) || !heldThresh) return;

	if(i == BTN_LEFT && lv_slider_get_value(slider) > 0){
		lv_slider_set_value(slider, lv_slider_get_value(slider) - 1, LV_ANIM_ON);
	}else if(i == BTN_RIGHT && lv_slider_get_value(slider) < 45){
		lv_slider_set_value(slider, lv_slider_get_value(slider) + 1, LV_ANIM_ON);
	}
	setColor(lv_slider_get_value(slider) * 8);
}

void ColorBox::buttonReleased(uint i){
	if(!lv_obj_has_state(slider, LV_STATE_EDITED) && heldThresh) return;

	if(i == BTN_LEFT || i == BTN_RIGHT){
		heldThresh = false;
	}
}

void ColorBox::buttonHeld(uint i){
	if(!lv_obj_has_state(slider, LV_STATE_EDITED) && !heldThresh) return;

	if(i == BTN_LEFT || i == BTN_RIGHT){
		heldThresh = true;
	}
}

uint16_t ColorBox::getHue() const{
	return hue;
}
