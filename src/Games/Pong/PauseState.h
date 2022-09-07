#ifndef BONK_PAUSESTATE_H
#define BONK_PAUSESTATE_H

#include "State.hpp"

namespace Bonk {
	class PauseState : public State {
	public:
		PauseState(Sprite* sprite);
		void update(uint _time, Bonk& game) override;
		void draw() override;
		void start(Bonk& game) override;
		void stop() override;

	private:
		static PauseState* instance;
	};
};

#endif