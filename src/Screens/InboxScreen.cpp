#include <Pins.hpp>
#include "InboxScreen.h"
#include "../Elements/UserWithMessage.h"
#include "../Fonts/font.h"
#include "../Storage/Storage.h"
#include "ConvoScreen.h"
#include "../Elements/ListItem.h"
#include "PairScreen.h"
#include "LockScreen.h"
#include <Input/Input.h>

InboxScreen::InboxScreen() : LVScreen(), apop(this){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);

	if(Storage.Friends.all().size() == 1){
		auto listItem = new ListItem(obj, "Add friend", 1);
		lv_group_add_obj(inputGroup, listItem->getLvObj());
		lv_obj_add_event_cb(listItem->getLvObj(), [](lv_event_t* event){
			LVScreen* screen = new PairScreen();
			static_cast<LVScreen*>(lv_event_get_user_data(event))->push(screen);
		}, LV_EVENT_PRESSED, this);
		auto label = lv_label_create(obj);
		lv_label_set_text(label, "You don't have any friends added yet.\n\nPress ENTER to pair with a friend.");
		lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
		lv_obj_set_style_text_color(label, lv_color_white(), 0);
		lv_obj_set_style_text_font(label, &pixelbasic7, 0);
		lv_obj_set_style_pad_gap(obj, 30, 0);
		lv_obj_set_width(label, lv_pct(100));
		lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
		return;
	}

	std::vector<UID_t> frens = Storage.Friends.all();
	params.reserve(frens.size());

	for(UID_t uid : frens){
		if(uid == ESP.getEfuseMac()) continue;

		Friend fren = Storage.Friends.get(uid);
		if(fren.uid == 0) continue;

		params.push_back({ uid, this });

		auto user = new UserWithMessage(obj, fren);
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

void InboxScreen::onStart(){
	apop.start();
	Input::getInstance()->addListener(this);
}

void InboxScreen::onStarting(){
	if(Storage.Friends.all().size() == 2 && !userElements.size()){
		lv_obj_clean(obj);
		std::vector<UID_t> frens = Storage.Friends.all();
		params.reserve(frens.size());

		for(UID_t uid : frens){
			if(uid == ESP.getEfuseMac()) continue;

			Friend fren = Storage.Friends.get(uid);
			if(fren.uid == 0) continue;

			params.push_back({ uid, this });

			auto user = new UserWithMessage(obj, fren);
			lv_group_add_obj(inputGroup, user->getLvObj());
			lv_obj_add_flag(user->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);

			lv_obj_add_event_cb(user->getLvObj(), [](lv_event_t* event){
				LaunchParams* params = static_cast<LaunchParams*>(event->user_data);
				params->ctx->openConvo(params->uid);
			}, LV_EVENT_CLICKED, &params.back());
			userElements.push_back(user);
		}
	}
	for(auto user : userElements){
		user->updateText();
	}
}

void InboxScreen::onStop(){
	apop.stop();
	Input::getInstance()->removeListener(this);
}

void InboxScreen::buttonPressed(uint i){
	if(i == BTN_R){
		LockScreen::activate(this);
	}
}
