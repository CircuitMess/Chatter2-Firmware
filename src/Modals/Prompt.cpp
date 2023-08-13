#include "Prompt.h"
#include "../Fonts/font.h"
#include <Chatter.h>
#include <Settings.h>

Prompt::Prompt(LVScreen* parent, const char* text) : LVModal(parent){

	lv_style_init(&btnDefault);
	lv_style_set_pad_all(&btnDefault, 3);

	lv_style_init(&btnFocused);
	lv_style_set_pad_all(&btnFocused, 2);
	lv_style_set_border_opa(&btnFocused, LV_OPA_100);
	lv_style_set_border_color(&btnFocused, lv_color_white());
	lv_style_set_border_width(&btnFocused, 1);
	lv_style_set_bg_color(&btnFocused, lv_color_make(205, 96, 147));
	lv_style_set_bg_opa(&btnFocused, LV_OPA_100);


	lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);

	lv_obj_t* avatarContainer = lv_obj_create(obj);
	lv_obj_clear_flag(avatarContainer, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_size(avatarContainer, lv_pct(100), LV_SIZE_CONTENT);
	lv_obj_set_style_pad_all(avatarContainer, 7, 0);
	lv_obj_set_style_border_width(avatarContainer, 1, 0);
	lv_obj_set_style_border_color(avatarContainer, lv_color_white(), 0);
	lv_obj_set_style_border_opa(avatarContainer, LV_OPA_100, 0);
	lv_obj_set_style_border_side(avatarContainer, LV_BORDER_SIDE_FULL & ~LV_BORDER_SIDE_BOTTOM, 0);

	lv_obj_set_style_bg_color(avatarContainer, lv_color_make(86, 64, 100), 0);
	lv_obj_set_style_bg_opa(avatarContainer, LV_OPA_100, 0);

	lv_obj_t* label = lv_label_create(avatarContainer);
	lv_obj_set_width(label, lv_pct(100));
	lv_label_set_text(label, text);
	lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_text_font(label, &pixelbasic7, 0);
	lv_obj_set_style_text_color(label, lv_color_white(), 0);
	lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

	lv_obj_t* labelContainer = lv_obj_create(obj);
	lv_obj_clear_flag(labelContainer, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_size(labelContainer, lv_pct(100), LV_SIZE_CONTENT);
	lv_obj_set_style_border_width(labelContainer, 1, 0);
	lv_obj_set_style_border_color(labelContainer, lv_color_white(), 0);
	lv_obj_set_style_border_opa(labelContainer, LV_OPA_100, 0);
	lv_obj_set_style_bg_color(labelContainer, lv_color_make(86, 64, 100), 0);
	lv_obj_set_style_bg_opa(labelContainer, LV_OPA_100, 0);

	yesBtn = lv_btn_create(labelContainer);
	lv_obj_align(yesBtn, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_size(yesBtn, lv_pct(50), LV_SIZE_CONTENT);
	lv_obj_t* yesLabel = lv_label_create(yesBtn);
	lv_obj_set_style_text_font(yesLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(yesLabel, lv_color_white(), 0);
	lv_obj_align(yesLabel, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(yesLabel, "YES");
	lv_obj_set_height(yesBtn, LV_SIZE_CONTENT);

	noBtn = lv_btn_create(labelContainer);
	lv_obj_align(noBtn, LV_ALIGN_RIGHT_MID, 0, 0);
	lv_obj_set_size(noBtn, lv_pct(50), LV_SIZE_CONTENT);
	lv_obj_t* noLabel = lv_label_create(noBtn);
	lv_obj_set_style_text_font(noLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(noLabel, lv_color_white(), 0);
	lv_obj_align(noLabel, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(noLabel, "NO");
	lv_obj_set_height(noBtn, LV_SIZE_CONTENT);

	lv_obj_set_height(labelContainer, LV_SIZE_CONTENT);

	lv_obj_add_style(yesBtn, &btnDefault, LV_STATE_DEFAULT);
	lv_obj_add_style(noBtn, &btnDefault, LV_STATE_DEFAULT);
	lv_obj_add_style(yesBtn, &btnFocused, LV_STATE_FOCUSED);
	lv_obj_add_style(noBtn, &btnFocused, LV_STATE_FOCUSED);

	Serial.println("add yes/no buttons to modal group");
	lv_group_add_obj(inputGroup, yesBtn);
	lv_group_add_obj(inputGroup, noBtn);
	lv_group_focus_obj(noBtn);


	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_height(container, LV_SIZE_CONTENT);

	lv_obj_add_event_cb(yesBtn, [](lv_event_t* event){
		auto modal = static_cast<LVModal*>(lv_event_get_user_data(event));
		modal->stop();
		lv_event_send(modal->getLvObj(), EV_PROMPT_YES, modal);
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(noBtn, [](lv_event_t* event){
		auto modal = static_cast<LVModal*>(lv_event_get_user_data(event));
		modal->stop();
		lv_event_send(modal->getLvObj(), EV_PROMPT_NO, modal);
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(yesBtn, [](lv_event_t* event){
		auto modal = static_cast<LVModal*>(lv_event_get_user_data(event));
		if(lv_event_get_key(event) == LV_KEY_ESC){
			modal->stop();
			lv_event_send(modal->getLvObj(), EV_PROMPT_NO, modal);
		}
	}, LV_EVENT_KEY, this);

	lv_obj_add_event_cb(noBtn, [](lv_event_t* event){
		auto modal = static_cast<LVModal*>(lv_event_get_user_data(event));
		if(lv_event_get_key(event) == LV_KEY_ESC){
			modal->stop();
			lv_event_send(modal->getLvObj(), EV_PROMPT_NO, modal);
		}
	}, LV_EVENT_KEY, this);

}

Prompt::~Prompt(){
	lv_style_reset(&btnDefault);
	lv_style_reset(&btnFocused);
}
