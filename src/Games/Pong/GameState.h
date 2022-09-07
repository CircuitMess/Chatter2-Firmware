#ifndef BONK_GAMESTATE_H
#define BONK_GAMESTATE_H

#include "State.hpp"

namespace Bonk {
	class GameState : public State {
	public:
		GameState(Sprite* sprite);
		virtual ~GameState();
		void update(uint _time, Bonk& game) override;
		void draw() override;
		void start(Bonk& game) override;
		void stop() override;
		bool rectRect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

	private:
		//player variables
		uint8_t playerScore;
		float playerHeight;
		float playerWidth;
		float playerX;
		float playerY;
		float playerSpeedY;
		//oponent variables
		uint8_t opponentScore;
		float opponentHeight;
		float opponentWidth;
		float opponentX;
		float opponentY;
		float opponentSpeedY;
		//ball variables
		float ballSize;
		float ballX;
		float ballY;
		float ballSpeedX;
		float ballSpeedY;

		bool playerUp = false;
		bool playerDown = false;

		bool winCondition;
		uint deathTimer;

		static GameState* instance;
	};
};

#endif