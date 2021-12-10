#include "User.h"

User::User(LVObject* parent, uint16_t hue, const String& name) : LVObject(parent), hue(hue), name(name){
	// Flex layout
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	// Selectors...
	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;
	lv_style_selector_t selPress = LV_PART_MAIN | LV_STATE_PRESSED;

	// Basic styling: radius, padding, border
	lv_obj_set_style_radius(obj, 0, sel);
	lv_obj_set_style_pad_hor(obj, 3, sel);
	lv_obj_set_style_pad_ver(obj, 5, sel);
	lv_obj_set_style_border_width(obj, 0, sel);
	lv_obj_set_style_outline_width(obj, 0, sel);

	// width / height
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_width(obj, lv_pct(100));

	// Default style
	lv_style_init(&styleDef);
	lv_style_set_bg_color(&styleDef, lv_color_hsv_to_rgb(hue, 60, 85));
	lv_obj_add_style(obj, &styleDef, sel);

	// Focused style
	lv_style_init(&styleFocus);
	lv_style_set_bg_color(&styleFocus, lv_color_hsv_to_rgb(hue, 85, 100));
	lv_obj_add_style(obj, &styleFocus, selFocus);

	// Transition
	lv_style_prop_t trans_props[] = { LV_STYLE_BG_COLOR, static_cast<lv_style_prop_t>(0) };
	static lv_style_transition_dsc_t trans;
	lv_style_transition_dsc_init(&trans, trans_props, lv_anim_path_ease_in_out, 1200, 0, nullptr);
	lv_style_set_transition(&styleDef, &trans);
	lv_style_set_transition(&styleFocus, &trans);

	// Avatar
	lv_obj_t* avatar = lv_obj_create(obj);
	lv_obj_set_size(avatar, 24, 24);
	lv_obj_set_style_bg_color(avatar, lv_color_hsv_to_rgb(hue, 100, 100), sel);
	lv_obj_set_style_radius(avatar, LV_RADIUS_CIRCLE, sel);
	lv_obj_set_style_border_width(avatar, 1, sel);
	lv_obj_set_style_border_color(avatar, lv_color_black(), sel);
	lv_obj_set_scrollbar_mode(avatar, LV_SCROLLBAR_MODE_OFF);

	// Name
	lv_obj_t* label = lv_label_create(obj);
	lv_label_set_text(label, this->name.c_str());
	lv_obj_set_flex_grow(label, 1);
}
