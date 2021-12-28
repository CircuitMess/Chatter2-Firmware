#include "Avatar.h"
#include <Arduino.h>

Avatar::Avatar(lv_obj_t* parent, uint8_t index){

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	obj = lv_img_create(parent);
//	obj = lv_img_create(parent);

	changeImage(index);
/*	lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, sel);
	lv_obj_set_style_border_width(obj, 1, sel);
	lv_obj_set_size(obj, 14, 14);*/

	lv_obj_set_size(obj, 14, 14);
	lv_obj_set_style_bg_color(obj, lv_color_hsv_to_rgb(50, 100, 100), sel);
	lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, sel);
	lv_obj_set_style_bg_opa(obj, LV_OPA_100, sel);
	lv_obj_set_style_border_width(obj, 1, sel);
	lv_obj_set_style_border_opa(obj, LV_OPA_100, sel);
	lv_obj_set_style_border_color(obj, lv_color_black(), sel);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);

//	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
}

void Avatar::changeImage(uint8_t index){
	char imgPath[50];
	sprintf(imgPath, "S:/avatars/%d.bin", index);
	lv_img_set_src(obj, imgPath);
}
