#include "UserWithMessage.h"

UserWithMessage::UserWithMessage(lv_obj_t* parent, const Profile &profile, const char* messageText) : User(parent, profile){
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

	//new layout for name and message
	lv_obj_t* nameMessageLayout = lv_obj_create(obj);
	lv_obj_set_layout(nameMessageLayout, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(nameMessageLayout, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(nameMessageLayout, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_parent(name, nameMessageLayout);
	lv_obj_set_style_pad_all(nameMessageLayout, 1, 0);
	lv_obj_set_flex_grow(nameMessageLayout, 1);

	//message label
	lv_obj_t* message = lv_label_create(nameMessageLayout);
	lv_label_set_long_mode(message, LV_LABEL_LONG_DOT);
	lv_obj_set_height(message, 12);
	lv_obj_set_style_opa(message, LV_OPA_90, 0);
	lv_obj_set_style_text_color(message, lv_color_white(), 0);
	lv_label_set_text(message, messageText);
	lv_obj_set_style_text_font(message, &pixelbasic_7, 0);
	lv_obj_set_height(name, 12);
	lv_obj_set_height(nameMessageLayout, 25);

	lv_obj_update_layout(nameMessageLayout);
	lv_obj_set_width(message, lv_obj_get_width(nameMessageLayout));

	//fixes border drawing issues
	lv_obj_add_event_cb(obj, [](lv_event_t* e){
		lv_obj_invalidate(lv_obj_get_parent(lv_event_get_target(e)));
	}, LV_EVENT_FOCUSED, nullptr);
}
