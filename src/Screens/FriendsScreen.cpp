#include "FriendsScreen.h"
#include "../Model/Profile.hpp"
#include "../font.h"
#include "../Types.hpp"
#include "../Storage/Storage.h"
#include "../User.h"
#include "../ListItem.h"

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

	std::vector<UID_t> friends = Storage.Friends.all();
	for(UID_t uid : friends){
		Friend fren = Storage.Friends.get(uid);
		User* user = new User(obj, fren.profile);
		lv_group_add_obj(inputGroup, user->getLvObj());
		lv_obj_add_flag(user->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);
	}
}

void FriendsScreen::onStart(){
	apop.start();
}

void FriendsScreen::onStop(){
	apop.stop();
}
