#include "InboxScreen.h"
#include "../UserWithMessage.h"
#include "../font.h"
#include "../Model/Convo.hpp"
#include "../Storage/Storage.h"
#include "ConvoScreen.h"

InboxScreen::InboxScreen() : LVScreen(), apop(this){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);
	lv_obj_set_style_bg_img_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_bg_img_src(obj, "S:/bg.bin", 0);

	newConvoLayout = lv_obj_create(obj);
	lv_group_add_obj(inputGroup, newConvoLayout);
	lv_obj_set_width(newConvoLayout, lv_pct(100));
	lv_obj_set_height(newConvoLayout, LV_SIZE_CONTENT);
	lv_obj_set_layout(newConvoLayout, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(newConvoLayout, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(newConvoLayout, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(newConvoLayout, 3, 0);
	lv_obj_set_style_border_width(newConvoLayout, 1, 0);
	lv_obj_set_style_border_color(newConvoLayout, lv_color_white(), 0);
	lv_obj_set_style_border_opa(newConvoLayout, LV_OPA_100, 0);
	lv_obj_set_style_pad_all(newConvoLayout, 2, 0);

	lv_obj_set_style_bg_color(newConvoLayout, lv_color_black(), LV_STATE_FOCUSED | LV_PART_MAIN);
	lv_obj_set_style_bg_opa(newConvoLayout, LV_OPA_90, LV_STATE_FOCUSED | LV_PART_MAIN);
	lv_obj_set_style_border_width(newConvoLayout, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
	lv_obj_set_style_pad_all(newConvoLayout, 1, LV_PART_MAIN | LV_STATE_FOCUSED);

	plusImg = lv_img_create(newConvoLayout);
	lv_img_set_src(plusImg, LV_SYMBOL_PLUS);
	lv_obj_set_style_text_color(plusImg, lv_color_white(), 0);

	newConvoLabel = lv_label_create(newConvoLayout);
	lv_label_set_text(newConvoLabel, "New conversation");
	lv_obj_set_style_text_font(newConvoLabel, &pixelbasic_7, 0);
	lv_obj_set_style_text_color(newConvoLabel, lv_color_white(), 0);

	lv_obj_add_event_cb(newConvoLayout, [](lv_event_t* event){
		static_cast<InboxScreen*>(event->user_data)->newConvo();
	}, LV_EVENT_CLICKED, this);

	std::vector<UID_t> convos = Storage.Convos.all();
	params.reserve(convos.size());

	for(UID_t uid : convos){
		Convo convo = Storage.Convos.get(uid);
		if(convo.uid == 0) continue;

		std::string text = "";
		if(!convo.messages.empty()){
			Message msg = Storage.Messages.get(convo.messages.back());
			if(msg.uid == 0) continue;
			text = msg.getText();
		}

		Friend fren = Storage.Friends.get(uid);
		if(fren.uid == 0) continue;

		params.push_back({ uid, this });

		auto user = new UserWithMessage(obj, fren.profile, text);
		lv_group_add_obj(inputGroup, user->getLvObj());
		lv_obj_add_flag(user->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);

		lv_obj_add_event_cb(user->getLvObj(), [](lv_event_t* event){
			LaunchParams* params = static_cast<LaunchParams*>(event->user_data);
			params->ctx->openConvo(params->uid);
		}, LV_EVENT_CLICKED, &params.back());

		userElements.push_back(user);
	}
}

void InboxScreen::openConvo(UID_t uid){
	auto screen = new ConvoScreen(uid);
	push(screen);
}

void InboxScreen::newConvo(){
	Serial.println("new convo");
	//TODO - staviti otvaranje menija za odabir prijatelja
}

void InboxScreen::onStart(){
	apop.start();
}

void InboxScreen::onStop(){
	apop.stop();
}
