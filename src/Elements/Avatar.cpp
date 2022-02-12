#include "Avatar.h"
#include <Arduino.h>

Avatar::Avatar(lv_obj_t* parent, uint8_t index, bool large) : LVObject(parent), large(large){

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocused = LV_PART_MAIN | LV_STATE_FOCUSED;
	img = lv_img_create(obj);
	lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

	changeImage(index);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);

	lv_obj_set_style_radius(img, LV_RADIUS_CIRCLE, sel);
	lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, sel);
	lv_obj_set_style_pad_all(obj, 2, sel);
	lv_obj_set_style_border_width(obj, 0, sel);
	lv_obj_set_style_bg_opa(obj, LV_OPA_0, sel);

	lv_obj_set_style_border_color(obj, lv_color_white(), selFocused);
	lv_obj_set_style_border_width(obj, 2, selFocused);
	lv_obj_set_style_border_opa(obj, LV_OPA_100, selFocused);
	lv_obj_set_style_pad_all(obj, 0, selFocused);
}

void Avatar::changeImage(uint8_t index){
	if(index > 14){
		lv_obj_set_style_bg_opa(img, LV_OPA_100, 0);
		lv_obj_set_style_bg_color(img, lv_color_white(), 0);
		lv_obj_set_size(img, large ? 42 : 14, large ? 42 : 14);
		lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
		return;
	}

	char imgPath[50];
	sprintf(imgPath, large ? "S:/Avatars/large/%d.bin" : "S:/Avatars/small/%d.bin", index + 1);
	lv_img_set_src(img, imgPath);
}
