#include "ConvoMessage.h"
#include <Arduino.h>

lv_style_t borderStyle;
ConvoMessage::ConvoMessage(lv_obj_t* parent, bool outgoing, const char* content, uint8_t bgColor, bool delivered) : LVObject(parent){

//	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
//	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
//	lv_obj_set_flex_align(obj, outgoing ? LV_FLEX_ALIGN_END: LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_width(obj, lv_pct(100));
	lv_obj_set_height(obj, LV_SIZE_CONTENT);

	lv_style_init(&borderStyle);
	lv_style_set_border_opa(&borderStyle, LV_OPA_100);
	lv_style_set_border_color(&borderStyle, lv_color_black());
	lv_style_set_border_width(&borderStyle, 1);

	label = lv_label_create(obj);
	lv_obj_align(label, outgoing ? LV_ALIGN_RIGHT_MID : LV_ALIGN_LEFT_MID, 0, 0);
	lv_label_set_text(label, content);

	lv_obj_set_height(label, LV_SIZE_CONTENT);
	lv_obj_update_layout(parent);

	Serial.println(lv_obj_get_height(label));
	Serial.println(lv_obj_get_height(obj));


	lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
	lv_obj_set_scrollbar_mode(label, LV_SCROLLBAR_MODE_OFF);

	lv_obj_add_style(label, &borderStyle, 0);
	lv_obj_add_style(obj, &borderStyle, 0);

	lv_style_init(&defaultStyle);
	lv_style_set_bg_opa(&defaultStyle, LV_OPA_100);
	lv_style_set_text_color(&defaultStyle, outgoing ? lv_color_hsv_to_rgb(bgColor, 70, 90) :  lv_color_white());
	lv_style_set_bg_color(&defaultStyle, outgoing ? lv_color_hsv_to_rgb(0, 0, 100) : lv_color_hsv_to_rgb(bgColor, 60, 85));
	lv_style_set_radius(&defaultStyle, 3);
	lv_style_set_max_width(&defaultStyle, lv_pct(50));
	lv_style_set_pad_all(&defaultStyle, 2);

	lv_style_init(&focusedStyle);
	lv_style_set_bg_color(&focusedStyle, outgoing ? lv_color_white() : lv_color_hsv_to_rgb(bgColor, 100, 100));

	lv_obj_add_style(label, &defaultStyle, LV_STATE_DEFAULT | LV_PART_MAIN);
	lv_obj_add_style(label, &focusedStyle, LV_STATE_FOCUSED | LV_PART_MAIN);
}
