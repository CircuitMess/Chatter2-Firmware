#include "User.h"
#include "pixelbasic_7.hpp"
User::User(lv_obj_t* parent, const Profile &profile) : LVObject(parent){
	// Flex layout
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	// Selectors...
	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;
	lv_style_selector_t selPress = LV_PART_MAIN | LV_STATE_PRESSED;

	lv_obj_set_style_pad_gap(obj, 7, sel);

	// width / height
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_width(obj, lv_pct(100));

	// Default style
	lv_style_init(&styleDef);
	lv_style_set_bg_opa(&styleDef, LV_OPA_100);
	lv_style_set_bg_color(&styleDef, lv_color_hsv_to_rgb(profile.color, 60, 85));
	lv_style_set_border_color(&styleDef, lv_color_white());
	lv_style_set_border_opa(&styleDef, LV_OPA_100);
	lv_style_set_border_width(&styleDef, 1);
	lv_style_set_pad_all(&styleDef, 2);
	lv_obj_add_style(obj, &styleDef, sel);


	// Focused style
	lv_style_init(&styleFocus);
	lv_style_set_bg_color(&styleFocus, lv_color_hsv_to_rgb(profile.color, 85, 100));
	lv_obj_add_style(obj, &styleFocus, selFocus);
	lv_style_set_border_color(&styleDef, lv_color_white());
	lv_style_set_border_opa(&styleDef, LV_OPA_100);
	lv_style_set_border_width(&styleFocus, 2);
	lv_style_set_pad_all(&styleFocus, 1);


	// Transition
	lv_style_prop_t trans_props[] = { LV_STYLE_BG_COLOR, static_cast<lv_style_prop_t>(0) };
	static lv_style_transition_dsc_t trans;
	lv_style_transition_dsc_init(&trans, trans_props, lv_anim_path_ease_in_out, 1200, 0, nullptr);
	lv_style_set_transition(&styleDef, &trans);
	lv_style_set_transition(&styleFocus, &trans);

	// Avatar
	lv_obj_t* avatar = lv_obj_create(obj);
	lv_obj_set_size(avatar, 14, 14);
	lv_obj_set_style_bg_opa(avatar, LV_OPA_100, sel);
	lv_obj_set_style_bg_color(avatar, lv_color_hsv_to_rgb(profile.color, 100, 100), sel);
	lv_obj_set_style_radius(avatar, LV_RADIUS_CIRCLE, sel);
/*	lv_obj_set_style_border_width(avatar, 1, sel);
	lv_obj_set_style_border_color(avatar, lv_color_black(), sel);*/
	lv_obj_set_scrollbar_mode(avatar, LV_SCROLLBAR_MODE_OFF);

	// Name
	lv_obj_t* label = lv_label_create(obj);
	lv_obj_set_style_text_font(label, &pixelbasic_7, sel);
	lv_label_set_text(label, profile.nickname);
	lv_obj_set_flex_grow(label, 1);
}
