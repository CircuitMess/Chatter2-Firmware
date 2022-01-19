#include "ColorBoxLVGL.h"


ColorBox::ColorBox(lv_obj_t* parent, uint16_t hue) : LVObject(parent), color(color){
	lv_obj_set_size(obj, 50, 20);
	lv_obj_set_style_bg_opa(obj, 0, 0);
	static const lv_style_prop_t props[] = {LV_STYLE_BG_COLOR};

	static lv_style_t style_colorPicker;
	static lv_style_t style_knob;

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;

	lv_style_init(&style_knob);
	lv_style_set_bg_opa(&style_knob, 0);
	lv_style_set_bg_img_src(&style_knob, "S:/arrowKnob.bin");

	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

	slider = lv_slider_create(obj);
	lv_obj_add_flag(slider, LV_OBJ_FLAG_HIDDEN);
	lv_obj_remove_style_all(slider);        /*Remove the styles coming from the theme*/
	lv_obj_set_width(slider, 46);
	lv_obj_set_height(slider, 8);
	lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);
	lv_obj_set_style_pad_hor(slider,1,0);
	lv_style_set_width(&style_knob, 1);
	lv_style_set_height(&style_knob, 1);
	lv_obj_center(slider);
	//lv_slider_set_value(slider, 20, LV_ANIM_OFF);

	lv_style_init(&style_colorPicker);
	lv_style_set_bg_opa(&style_colorPicker, selFocus);
	lv_style_set_bg_img_src(&style_colorPicker, "S:/colorPicker.bin");

	colorStrip = lv_obj_create(obj);
	lv_obj_set_size(colorStrip, 46, 8);
	lv_obj_set_style_bg_color(colorStrip, lv_color_hsv_to_rgb(0, 100, 100), sel);

	lv_obj_set_style_border_width(colorStrip, 1, selFocus);
	lv_obj_set_style_border_color(colorStrip, lv_color_white(), selFocus);
	lv_obj_add_style(colorStrip, &style_colorPicker, selFocus);
	lv_style_set_width(&style_colorPicker, 46);
	lv_style_set_height(&style_colorPicker, 8);

	//lv_img_set_src(colorStrip, "S:/colorPicker.bin");
	lv_obj_align(colorStrip, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_opa(colorStrip, LV_OPA_COVER, selFocus);
	lv_obj_set_size(colorStrip, 46, 8);

	lv_group_add_obj((lv_group_t*) lv_obj_get_group(obj), slider);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		static_cast<ColorBox*>(lv_event_get_user_data(event))->selected();
	}, LV_EVENT_FOCUSED, this);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		static_cast<ColorBox*>(lv_event_get_user_data(event))->deselected();
	}, LV_EVENT_DEFOCUSED, this);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		static_cast<ColorBox*>(lv_event_get_user_data(event))->clicked();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		uint32_t c = lv_event_get_key(event);
		auto sliderEvent = static_cast<ColorBox*>(event->user_data);
		if(c == LV_KEY_ESC){
			sliderEvent->exit();
		}
	}, LV_EVENT_KEY, this);
}

void ColorBox::selected(){
	lv_obj_set_style_border_width(colorStrip,1,0);
	lv_obj_set_style_border_color(colorStrip,lv_color_white(),0);
	lv_group_set_editing((lv_group_t*)lv_obj_get_group(slider), false);

}

void ColorBox::deselected(){
	lv_obj_set_style_border_width(colorStrip,0,0);
}

void ColorBox::clicked(){
	lv_obj_clear_flag(slider, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_state(colorStrip, LV_STATE_FOCUSED);
	lv_obj_add_state(slider, LV_STATE_EDITED);
	lv_group_focus_obj(slider);
	lv_group_set_editing((lv_group_t*)lv_obj_get_group(slider), true);
}

void ColorBox::exit(){
	lv_obj_add_flag(slider, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_state(colorStrip, LV_STATE_FOCUSED);
	lv_group_focus_obj(obj);
	lv_group_set_editing((lv_group_t*)lv_obj_get_group(slider), false);
}