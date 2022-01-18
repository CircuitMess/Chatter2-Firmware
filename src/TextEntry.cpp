#include "TextEntry.h"

TextEntry::TextEntry(lv_obj_t* parent, std::string text) : LVObject(parent), text(text){
	// Selectors
	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;

	lv_obj_set_height(obj, 20);
	lv_obj_set_width(obj, lv_pct(100));
	lv_obj_set_style_bg_color(obj,lv_palette_main(LV_PALETTE_RED),sel);
	lv_obj_set_style_bg_opa(obj, LV_OPA_100,sel);

	lv_style_init(&styleDef);
	lv_obj_add_style(obj, &styleDef, sel);

	// Focused style
	lv_style_init(&styleFocus);
	lv_obj_t* entry = lv_textarea_create(obj);
	lv_textarea_set_one_line(entry, true);

	lv_style_init(&styleFocus);
	lv_obj_add_style(entry, &styleFocus, LV_PART_CURSOR);
	lv_obj_add_state(entry,LV_STATE_FOCUSED);//da kursor blinka cijelo vrijeme
	lv_obj_set_height(entry, 20);
	lv_obj_set_width(entry, lv_pct(100));
	lv_obj_set_style_bg_color(entry,lv_color_white(),sel);
	lv_obj_set_style_bg_opa(entry, LV_OPA_100,sel);
	lv_obj_set_style_pad_left(entry,2,sel);

	if(text.empty()){
		lv_textarea_set_placeholder_text(entry, "Type here...");
	}else{
		lv_textarea_add_text(entry, text.c_str());
	}

	/*lv_obj_add_event_cb(entry, [](lv_event_t* event){
		lv_obj_add_state(lv_obj_get_child(lv_event_get_target(event), 0), LV_STATE_FOCUSED);
	}, LV_EVENT_FOCUSED, nullptr);

	lv_obj_add_event_cb(entry, [](lv_event_t* event){
		lv_obj_clear_state(lv_obj_get_child(lv_event_get_target(event), 0), LV_STATE_FOCUSED);
	}, LV_EVENT_DEFOCUSED, nullptr);*/
}


