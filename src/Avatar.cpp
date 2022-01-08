#include "Avatar.h"
#include <Arduino.h>

Avatar::Avatar(lv_obj_t* parent, uint8_t index, bool large) : large(large){

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocused = LV_PART_MAIN | LV_STATE_FOCUSED;
	obj = lv_img_create(parent);
	changeImage(index);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);

	lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, sel);
	lv_obj_set_style_pad_all(obj, 2, sel);

	lv_obj_set_style_border_color(obj, lv_color_black(), selFocused);
	lv_obj_set_style_border_width(obj, 2, selFocused);
	lv_obj_set_style_border_opa(obj, LV_OPA_100, selFocused);
	lv_obj_set_style_pad_all(obj, 0, selFocused);
}

void Avatar::changeImage(uint8_t index){
	char imgPath[50];
	sprintf(imgPath, large ? "S:/Avatars/large/%d.bin" : "S:/Avatars/small/%d.bin", index);
	lv_img_set_src(obj, imgPath);
}
