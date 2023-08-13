#include "PauseState.h"
#include <Input/Input.h>
#include "Bonk.h"
#include "bonk_title.hpp"
#include <Chatter.h>

Bonk::PauseState* Bonk::PauseState::instance = nullptr;
Bonk::PauseState::PauseState(Sprite* sprite) : State(sprite)
{
	instance = this;
}
void Bonk::PauseState::start(Bonk& _game)
{
	game = &_game;
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		instance->game->resumeGame();
	});
	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		instance->game->quitGame();
	});
}
void Bonk::PauseState::stop()
{
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
}
void Bonk::PauseState::draw()
{
	display->setTextDatum(textdatum_t::top_center);
	display->setFont(&fonts::Font0);
	display->setTextSize(2);
	display->setTextColor(TFT_WHITE);
	display->clear(TFT_BLACK);
	display->setFont(&fonts::Font2);
	display->setTextSize(2);
	display->drawString("Paused", display->width()/2, display->height()/2 - 30 );
	display->setTextSize(1);

	display->drawString("ENTER: Resume", display->width()/2, display->height()/2 + 10);
	display->drawString("BACK: Quit", display->width()/2, display->height()/2 + 26);
}
void Bonk::PauseState::update(uint _time, Bonk& game)
{
}