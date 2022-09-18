#ifndef BONK_H
#define BONK_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/Input.h>
#include <Loop/LoopManager.h>

#include <UI/Image.h>
#include <Display/Sprite.h>
#include "../GameEngine/Game.h"

namespace Bonk {
	class State;
	
	class Bonk : public Game {
	public:
		Bonk(GamesScreen* gamesScreen);
		virtual ~Bonk();
		void onStart() override;
		void onLoop(float dt) override;
		void onStop() override;

		void newGame();
		void pauseGame();
		void resumeGame();
		void quitGame();

		void play(std::initializer_list<Chirp> sound);
		void play(const Sound& sound);

	private:
		void draw();

		State* state;
		State* pausedGameState = nullptr;
		Sprite* canvas;
	};
};
#endif