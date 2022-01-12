#include "TextEntry.h"

TextEntry::TextEntry(lv_obj_t* parent, std::string text) : LVObject(parent), text(text){

	obj = lv_textarea_create(lv_scr_act());

	lv_style_init(&styleFocus);
	lv_obj_add_style(obj, &styleFocus, LV_PART_CURSOR);

	lv_obj_add_state(obj, LV_STATE_FOCUSED);
	lv_obj_set_height(obj, 20);
	lv_obj_set_width(obj, lv_pct(90));

	lv_obj_align(obj, LV_ALIGN_BOTTOM_LEFT, 2, 0);
	if(text.empty()){
		lv_textarea_set_placeholder_text(obj, "Type here...");
	}else{
		lv_textarea_add_text(obj, text.c_str());
	}
}


