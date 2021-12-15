#include "ListItem.h"

ListItem::ListItem(lv_obj_t* parent, uint16_t hue, uint8_t icon, std::string text) : LVObject(parent), icon(icon), text(text){

	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;
	lv_style_selector_t selPress = LV_PART_MAIN | LV_STATE_PRESSED;

	lv_obj_set_style_radius(obj, 0, sel);
	lv_obj_set_style_pad_hor(obj, 3, sel);
	lv_obj_set_style_pad_ver(obj, 5, sel);
	lv_obj_set_style_border_width(obj, 0, sel);
	lv_obj_set_style_outline_width(obj, 0, sel);

	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_width(obj, lv_pct(100));

	lv_style_init(&styleDef);
	lv_style_set_bg_color(&styleDef, lv_color_hsv_to_rgb(hue, 60, 85));
	lv_obj_add_style(obj, &styleDef, sel);

	lv_style_init(&styleFocus);
	lv_style_set_bg_color(&styleFocus, lv_color_hsv_to_rgb(hue, 100, 100));
	lv_style_set_pad_hor(&styleFocus,8);
	lv_obj_add_style(obj, &styleFocus, selFocus);

	if(icon != 0){
		lv_obj_t* avatar = lv_obj_create(obj);
		lv_obj_set_size(avatar, 14, 14);
		lv_obj_set_style_bg_color(avatar, lv_color_hsv_to_rgb(hue, 100, 100), sel);
		lv_obj_set_style_radius(avatar, LV_RADIUS_CIRCLE, sel);
		lv_obj_set_style_border_width(avatar, 1, sel);
		lv_obj_set_style_border_color(avatar, lv_color_black(), sel);
		lv_obj_set_scrollbar_mode(avatar, LV_SCROLLBAR_MODE_OFF);
	}

	lv_obj_t* label = lv_label_create(obj);
	lv_label_set_text(label, text.c_str());
	lv_obj_set_flex_grow(label, 1);

}
