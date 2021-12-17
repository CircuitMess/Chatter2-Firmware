#include "ColorBoxLVGL.h"


ColorBox::ColorBox(lv_obj_t* parent, uint16_t hue) : LVObject(parent), color(color){

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;
	lv_style_selector_t selPress = LV_PART_MAIN | LV_STATE_PRESSED;

	lv_obj_set_style_radius(obj, 0, sel);
	lv_obj_set_style_pad_hor(obj, 3, sel);
	lv_obj_set_style_pad_ver(obj, 5, sel);
	lv_obj_set_style_border_width(obj, 0, sel);
	lv_obj_set_style_outline_width(obj, 0, sel);

	lv_obj_set_height(obj, 13);
	lv_obj_set_width(obj, 55);

	lv_style_init(&styleDef);
	lv_style_set_bg_color(&styleDef, color);
	lv_obj_add_style(obj, &styleDef, sel);

	lv_style_init(&styleFocus);
	lv_style_set_bg_color(&styleFocus, lv_color_hsv_to_rgb(hue,60,60));
	lv_obj_add_style(obj, &styleFocus, selFocus);


}
