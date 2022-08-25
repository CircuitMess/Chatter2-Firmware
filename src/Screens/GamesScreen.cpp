#include "GamesScreen.h"
#include "../Model/Profile.hpp"
#include "../Fonts/font.h"
#include "../Types.hpp"
#include "../Storage/Storage.h"
#include "../Elements/User.h"
#include "../Elements/ListItem.h"
#include "PairScreen.h"
#include "ProfileScreen.h"

const GamesScreen::GameInfo GamesScreen::Games[] = {
		{ "Space Rocks", []() -> LVScreen* { return nullptr; } },
		{ "Space Rocks 2", []() -> LVScreen* { return nullptr; } },
};

GamesScreen::GamesScreen() : LVScreen(), apop(this){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);

	for(const auto& game : Games){
		auto listItem = new ListItem(obj, game.name);
		lv_group_add_obj(inputGroup, listItem->getLvObj());
		lv_obj_add_flag(listItem->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);

		lv_obj_add_event_cb(listItem->getLvObj(), [](lv_event_t* e){
			auto game = static_cast<GamesScreen::GameInfo*>(e->user_data);
			LVScreen* screen = game->launch();
			static_cast<LVScreen*>(lv_event_get_user_data(e))->push(screen);
		}, LV_EVENT_PRESSED, (void*) &game);
	}
}

void GamesScreen::onStart(){
	apop.start();
}

void GamesScreen::onStop(){
	apop.stop();
}