#include "TextEntry.h"
#include <ByteBoi.h>

static void textarea_event_handler(lv_event_t * e)
{
	lv_obj_t * ta = lv_event_get_target(e);
	LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
}

TextEntry::TextEntry(lv_obj_t* parent, std::string text) : LVObject(parent), text(text){

	obj = lv_textarea_create(lv_scr_act());
	lv_textarea_set_placeholder_text(obj, "Hello");
	lv_textarea_set_one_line(obj, true);
	lv_obj_add_event_cb(obj, textarea_event_handler, LV_EVENT_READY, obj);
	//lv_obj_set_style_border_width(obj, 2, LV_STATE_FOCUSED);
	lv_obj_set_style_border_width(obj, 0, LV_STATE_DEFAULT);

	lv_obj_add_state(obj, LV_STATE_FOCUSED);
	lv_obj_set_height(obj, 20);
	lv_obj_set_width(obj, lv_pct(40));

	lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);

	lv_textarea_add_text(obj,"text");
}


