#include "GamesScreen.h"
#include "../Model/Profile.hpp"
#include "../Fonts/font.h"
#include "../Types.hpp"
#include "../Storage/Storage.h"
#include "../Elements/User.h"
#include "../Elements/ListItem.h"
#include "PairScreen.h"
#include "ProfileScreen.h"
#include "../Games/Space/SpaceRocks.h"
#include "../FSLVGL.h"
#include "../Games/Invaders/SpaceInvaders.h"

const GamesScreen::GameInfo GamesScreen::Games[] = {
		{ "Space Rocks", [](GamesScreen* gamesScreen) -> Game* { return new SpaceRocks(gamesScreen); } },
		{ "Invaders", [](GamesScreen* gamesScreen) -> Game* { return new SpaceInvaders::SpaceInvaders(gamesScreen); } },
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
			auto screen = static_cast<GamesScreen*>(e->user_data);
			screen->stop();
			FSLVGL::unloadCache();

			printf("Starting game...\n");

			printf("Heap: %.3f kB\n", (double) ESP.getFreeHeap() / 1024.0);
			printf("Largest block: %ul B\n", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));

			auto gameId = lv_obj_get_index(e->target);
			auto game = Games[gameId].launch(screen);

			printf("Heap: %.3f kB\n", (double) ESP.getFreeHeap() / 1024.0);

			game->load();
			while(!game->isLoaded()){
				delay(1);
			}

			printf("Heap: %.3f kB\n", (double) ESP.getFreeHeap() / 1024.0);

			game->start();

			printf("Heap: %.3f kB\n", (double) ESP.getFreeHeap() / 1024.0);
			printf("Largest block: %ul B\n", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
		}, LV_EVENT_PRESSED, this);
	}
}

void GamesScreen::onStart(){
	apop.start();
}

void GamesScreen::onStop(){
	apop.stop();
}