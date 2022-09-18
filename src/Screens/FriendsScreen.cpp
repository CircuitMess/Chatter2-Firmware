#include <Pins.hpp>
#include "FriendsScreen.h"
#include "../Model/Profile.hpp"
#include "../Fonts/font.h"
#include "../Types.hpp"
#include "../Storage/Storage.h"
#include "../Elements/User.h"
#include "../Elements/ListItem.h"
#include "PairScreen.h"
#include "ProfileScreen.h"
#include "LockScreen.h"
#include <Input/Input.h>

FriendsScreen::FriendsScreen() : LVScreen(), apop(this){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);

	auto listItem = new ListItem(obj,"Add friend",1);
	lv_group_add_obj(inputGroup, listItem->getLvObj());
	lv_obj_add_flag(listItem->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	lv_obj_add_event_cb(listItem->getLvObj(), [](lv_event_t* e){
		LVScreen* screen = new PairScreen();
		static_cast<LVScreen*>(lv_event_get_user_data(e))->push(screen);
	}, LV_EVENT_PRESSED, this);
}

void FriendsScreen::onStart(){
	apop.start();
	Input::getInstance()->addListener(this);
}

void FriendsScreen::onStop(){
	apop.stop();
	Input::getInstance()->removeListener(this);
}

void FriendsScreen::onStarting(){
	for(auto el : elements){
		if(Storage.Friends.exists(el->getUID())) continue;

		auto it = std::find(elements.begin(), elements.end(), el);
		if(it == elements.end()) break;

		elements.erase(it);
		delete el;
		break;
	}
	for(UID_t uid : Storage.Friends.all()){
		if(uid == ESP.getEfuseMac()) continue;

		bool exists = false;
		for(auto el:elements){
			if(el->getUID() == uid){
				exists = true;
				break;
			}
		}
		if(exists) continue;

		Friend fren = Storage.Friends.get(uid);
		User* user = new User(obj, fren);
		lv_group_add_obj(inputGroup, user->getLvObj());
		lv_obj_add_flag(user->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);

		lv_obj_add_event_cb(user->getLvObj(), [](lv_event_t* e){
			LVScreen* screen = new ProfileScreen(*((UID_t*)lv_obj_get_user_data(lv_event_get_target(e))));
			static_cast<LVScreen*>(lv_event_get_user_data(e))->push(screen);
		}, LV_EVENT_PRESSED, this);

		elements.push_back(user);
	}
}

void FriendsScreen::buttonPressed(uint i){
	if(i == BTN_R){
		LockScreen::activate(this);
	}
}
