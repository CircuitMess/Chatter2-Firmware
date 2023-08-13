#include "TitleState.h"
#include <Input/Input.h>
#include "Bonk.h"
#include "bonk_title.hpp"
#include <Chatter.h>
#include <SD.h>

Bonk::TitleState* Bonk::TitleState::instance = nullptr;
Bonk::TitleState::TitleState(Sprite* sprite) : State(sprite)
{
	instance = this;
	titleCursor = 0;
	blinkState = 0;
}
Bonk::TitleState::~TitleState(){
	TitleState::stop();
}
void Bonk::TitleState::start(Bonk& _game)
{
	game = &_game;
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		switch (instance->titleCursor)
		{
		case 0:
			instance->game->play({{500, 700, 50}});
			instance->game->newGame();
			break;
			case 1:
				instance->game->play(Sound { Chirp { 400, 350, 50 }});
				instance->game->pop();
				return;

		default:
			break;
		}
	});
	Input::getInstance()->setBtnPressCallback(BTN_UP, [](){
		if(instance->titleCursor > 0){
			instance->titleCursor--;
			instance->game->play({{500, 500, 50}});
		}
	});
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, [](){
		if(instance->titleCursor < 1){
			instance->titleCursor++;
			instance->game->play({{500, 500, 50}});
		}
	});
	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		instance->game->play(Sound { Chirp { 400, 350, 50 }});
		instance->game->pop();
	});
}
void Bonk::TitleState::stop()
{
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
}
void Bonk::TitleState::draw()
{
	display->clear(TFT_BLACK);
	// drawBitmap(30, 14, bonk_title, TFT_WHITE, 2);
	display->drawBitmap(29, 8, bonk_title, 51, 28, TFT_DARKGREY, (uint8_t)2);
	display->drawBitmap(27, 6, bonk_title, 51, 28, TFT_WHITE, (uint8_t)2);
	display->setTextSize(1);
	display->setFont(&fonts::Font2);
	display->setTextColor(TFT_WHITE);
	display->setTextDatum(textdatum_t::top_center);
	display->drawString("START",display->width()/2, 75);
	display->drawString("QUIT", display->width()/2, 99);
	display->drawRect(30, 72 + titleCursor * 24, 100, 22, blinkState ? TFT_RED : TFT_BLACK);
	display->drawRect(31, 73 + titleCursor * 24, 98, 20, blinkState ? TFT_RED : TFT_BLACK);
}
void Bonk::TitleState::update(uint _time, Bonk& game)
{
	blinkMicros+=_time;
	if(blinkMicros > 200000)
	{
		blinkState = !blinkState;
		blinkMicros = 0;
	}
}