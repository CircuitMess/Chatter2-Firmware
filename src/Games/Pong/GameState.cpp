#include "GameState.h"
#include <Input/Input.h>
#include "Bonk.h"
#include "bonk_title.hpp"
#include <Chatter.h>
#include <SD.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

Bonk::GameState* Bonk::GameState::instance = nullptr;
Bonk::GameState::GameState(Sprite* sprite) : State(sprite)
{
	instance = this;

	//player variables
	playerScore = 0;
	playerHeight = 32;
	playerWidth = 6;
	playerX = 5;
	playerY = (sprite->height() - playerHeight) / 2;
	playerSpeedY = 60;
	//oponent variables
	opponentScore = 0;
	opponentHeight = 32;
	opponentWidth = 6;
	opponentX = sprite->width() - opponentWidth - 5;
	opponentY = (sprite->height() - opponentHeight) / 2;
	opponentSpeedY = 60;
	//ball variables
	ballSize = 12;
	ballX = sprite->width() - ballSize - opponentWidth - 1;
	ballY = (sprite->height() - ballSize) / 2;
	ballSpeedX = 90.0;
	ballSpeedY = 90.0;

	winCondition = 0;
	deathTimer = 0;
}
Bonk::GameState::~GameState(){
	GameState::stop();
}
void Bonk::GameState::start(Bonk& _game)
{
	game = &_game;

	Input::getInstance()->setBtnPressCallback(BTN_UP, [](){
		instance->playerUp = true;
	});

	Input::getInstance()->setBtnReleaseCallback(BTN_UP, [](){
		instance->playerUp = false;
	});

	Input::getInstance()->setBtnPressCallback(BTN_DOWN, [](){
		instance->playerDown = true;
	});

	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, [](){
		instance->playerDown = false;
	});

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		instance->game->pauseGame();
	});


}
void Bonk::GameState::stop()
{
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnReleaseCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnReleaseCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
}
void Bonk::GameState::draw()
{
	display->setTextDatum(textdatum_t::middle_center);
	display->setTextWrap(false, false);
	display->setTextSize(5);
	display->setTextFont(0);
	display->setTextColor(TFT_WHITE);
	display->clear(TFT_BLACK);
	
	if(winCondition)
	{
		display->clear(TFT_BLACK);
		display->setTextFont(2);
		display->setTextColor(TFT_WHITE);
		display->setTextSize(2);
		if(playerScore >= 3){
			display->drawString("PLAYER", display->width()/2, 30);
		}
		else if(opponentScore >= 3){
			display->drawString("CPU", display->width()/2, 30);
		}
		display->drawString("WINS", display->width()/2,60);
		if(deathTimer >= 2000000)
		{
			game->quitGame();
		}
		return;
	}

	//draw the score
	display->setTextSize(3);
	display->drawString(String(String(playerScore) + "      " + String(opponentScore)).c_str(), display->width() / 2, 32);
	//draw the ball
	display->fillRect(ballX, ballY, ballSize, ballSize, TFT_WHITE);
	//draw the player
	display->fillRect(playerX, playerY, playerWidth, playerHeight, TFT_RED);
	//draw the oponent
	display->fillRect(opponentX, opponentY, opponentWidth, opponentHeight, TFT_BLUE);
}
void Bonk::GameState::update(uint _time, Bonk& game)
{
	float dt = (float) _time / 1000000.0f;

	//reset score when 10 is reached
	if ((playerScore == 3) || (opponentScore == 3)) {
		winCondition = 1;

		if(!winNotified){
			winNotified = true;

			if(opponentScore == 3){
				game.play({{ 400, 300, 200 },
							{ 0,   0,   50 },
							{ 300, 200, 200 },
							{ 0,   0,   50 },
							{ 200, 50,  400 }});
			}else{
				game.play({{ 600, 400,  200 },
							{ 400, 1000, 200 }});
			}
		}
	}
	if(winCondition)
	{
		deathTimer+=_time;
		return;
	}

	//move the oponent
	if ((opponentY + (opponentHeight / 2)) < (ballY + (ballSize / 2))) { //if the ball is below the oponent
		opponentY += opponentSpeedY * dt; //move down
		opponentY = min(display->height() - opponentHeight, opponentY); //don't go out of the screen
	}
	else {
		opponentY -= opponentSpeedY * dt; //move up
		opponentY = max(0, opponentY); //don't go out of the screen
	}

	if(playerUp && !playerDown){
		playerY -= playerSpeedY * dt;
		playerY = max(0, playerY);
	}else if(playerDown && !playerUp){
		playerY += playerSpeedY * dt;
		playerY = min(display->height() - playerHeight, playerY);
	}


	//move the ball
	ballX = ballX + ballSpeedX * dt;
	ballY = ballY + ballSpeedY * dt;

	//check for ball collisions
	//collision with the top border
	if (ballY < 0) {
		ballY = 0;
		ballSpeedY = -ballSpeedY;
		//Playback.tone(300, 100);
	}
	//collision with the bottom border
	if ((ballY + ballSize) > display->height()) {
		ballY = display->height() - ballSize;
		ballSpeedY = -ballSpeedY;
		//Playback.tone(300, 100);

	}
	//collision with the player
	if (rectRect(ballX, ballY, ballSize, ballSize, playerX, playerY, playerWidth, playerHeight)) {
		ballX = playerX + playerWidth;
		ballSpeedX = -ballSpeedX;
		instance->game->play({{100, 100, 50}});
	}
	//collision with the oponent
	if (rectRect(ballX, ballY, ballSize, ballSize, opponentX, opponentY, opponentWidth, opponentHeight)) {
		ballX = opponentX - ballSize;
		ballSpeedX = -ballSpeedX;
		instance->game->play({{100, 100, 50}});
	}
	//collision with the left side
	if (ballX < 0) {
		opponentScore = opponentScore + 1;
		//mp.sound.playCancel();
		ballX = display->width() - ballSize - opponentWidth - 1;
		ballSpeedX = -abs(ballSpeedX);
		ballY = random(0, display->height() - ballSize);
		instance->game->play({{600, 200, 200}});
	}
	//collision with the right side
	if ((ballX + ballSize) > display->width()) {
		playerScore = playerScore + 1;
		//mp.sound.playOK();
		ballX = display->width() - ballSize - opponentWidth - 16; //place the ball on the oponent side
		ballSpeedX = -abs(ballSpeedX);
		ballY = random(0, display->height() - ballSize);
		instance->game->play({{200, 600, 200}});
	}
}
bool Bonk::GameState::rectRect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	if (x1 + w1 >= x2 && x1 <= x2 + w2 &&
		y1 + h1 >= y2 && y1 <= y2 + h2)
	{
		return true;
	}
	return false;
}