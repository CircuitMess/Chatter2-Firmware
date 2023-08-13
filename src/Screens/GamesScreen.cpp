#include <Pins.hpp>
#include <Chatter.h>
#include <SPIFFS.h>
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
#include "../Games/Snake/Snake.h"
#include "../Games/Pong/Bonk.h"
#include "../Elements/GameItem.h"
#include "LockScreen.h"

const GamesScreen::GameInfo GamesScreen::Games[] = {
		{ "Space rocks", "S:/Games/Icons/Space.bin", [](GamesScreen* gamesScreen) -> Game* { return new SpaceRocks(gamesScreen); }, "/Games/Space/splash.raw" },
		{ "Invaderz", "S:/Games/Icons/Invaders.bin", [](GamesScreen* gamesScreen) -> Game* { return new SpaceInvaders::SpaceInvaders(gamesScreen); } },
		{ "Snake", "S:/Games/Icons/Snake.bin", [](GamesScreen* gamesScreen) -> Game* { return new Snake::Snake(gamesScreen); } },
		{ "Bonk", "S:/Games/Icons/Pong.bin", [](GamesScreen* gamesScreen) -> Game* { return new Bonk::Bonk(gamesScreen); } },
};

GamesScreen::GamesScreen() : LVScreen(), apop(this){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);

	new ListItem(obj, "Games");

	for(const auto& game : Games){
		auto listItem = new GameItem(obj, game.name, game.icon);

		lv_group_add_obj(inputGroup, listItem->getLvObj());
		// lv_obj_add_flag(listItem->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);

		lv_obj_add_event_cb(listItem->getLvObj(), [](lv_event_t* e){
			auto screen = static_cast<GamesScreen*>(e->user_data);
			screen->stop();

			auto gameId = lv_obj_get_index(e->target) - 1;

			LoopManager::defer([gameId, screen](uint32_t dt){
				FSLVGL::unloadCache();

				auto info = Games[gameId];

				uint32_t splashStart = 0;
				if(info.splash){
					auto display = Chatter.getDisplay();
					display->getBaseSprite()->drawIcon(SPIFFS.open(info.splash), 0, 0, 160, 128);
					display->commit();
					splashStart = millis();
				}

				auto game = info.launch(screen);

				game->load();
				while(!game->isLoaded()){
					delay(1);
				}

				while(millis() - splashStart < 2000){
					delay(10);
				}

				game->start();
			});
		}, LV_EVENT_PRESSED, this);
	}
}

void GamesScreen::onStart(){
	apop.start();
	Input::getInstance()->addListener(this);
}

void GamesScreen::onStop(){
	apop.stop();
	Input::getInstance()->removeListener(this);
}

void GamesScreen::buttonPressed(uint i){
	if(i == BTN_R){
		LockScreen::activate(this);
	}
}
