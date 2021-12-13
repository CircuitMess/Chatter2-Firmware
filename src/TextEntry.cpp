#include "TextEntry.h"
#include <ByteBoi.h>

TextEntry::TextEntry(lv_obj_t* parent, std::string text) : LVObject(parent), text(text){

	obj = lv_textarea_create(lv_scr_act());
	lv_textarea_set_placeholder_text(obj, "Hello");
	lv_textarea_set_one_line(obj, true);
	//lv_obj_set_style_border_width(obj, 2, LV_STATE_FOCUSED);
	lv_obj_set_style_border_width(obj, 0, LV_STATE_DEFAULT);

	lv_style_init(&styleFocus);
	lv_obj_add_style(obj, &styleFocus, LV_PART_CURSOR);
	//lv_textarea_set_cursor_pos(obj,LV_PART_CURSOR);

	lv_obj_add_state(obj, LV_STATE_FOCUSED);
	lv_obj_set_height(obj, 20);
	lv_obj_set_width(obj, lv_pct(40));

	lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);

	lv_textarea_add_text(obj,"text");
}


