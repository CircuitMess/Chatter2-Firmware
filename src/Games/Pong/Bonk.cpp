#include <Chatter.h>
#include "Bonk.h"
#include "State.hpp"
#include "TitleState.h"
#include "GameState.h"
#include "PauseState.h"
#include "State.hpp"

Bonk::Bonk::Bonk(GamesScreen* gamesScreen) : Game(gamesScreen, "", {}), canvas(Chatter.getDisplay()->getBaseSprite()){
	state = new TitleState(canvas);
}

Bonk::Bonk::~Bonk(){
	delete state;
}

void Bonk::Bonk::draw()
{
	state->draw();
}
void Bonk::Bonk::onLoop(float dt)
{
	state->update(dt * 1000000.0f, *this);
	draw();
}
void Bonk::Bonk::onStart()
{
	state->start(*this);
}
void Bonk::Bonk::onStop()
{
	state->stop();
}
void Bonk::Bonk::newGame()
{
	state->stop();
	delete state;
	state = new GameState(canvas);
	state->start(*this);
}
void Bonk::Bonk::pauseGame()
{
	state->stop();
	pausedGameState = state;
	state = new PauseState(canvas);
	state->start(*this);
}
void Bonk::Bonk::resumeGame()
{
	state->stop();
	delete state;
	state = pausedGameState;
	pausedGameState = nullptr;
	state->start(*this);
}
void Bonk::Bonk::quitGame()
{
	delete pausedGameState;
	pausedGameState = nullptr;

	state->stop();
	delete state;
	state = new TitleState(canvas);
	state->start(*this);
}

void Bonk::Bonk::play(std::initializer_list<Chirp> sound){
	Audio.play(sound);
}

void Bonk::Bonk::play(const Sound& sound){
	Audio.play(sound);
}
