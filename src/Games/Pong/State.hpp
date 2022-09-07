#ifndef BONK_STATE_H
#define BONK_STATE_H

#include <Display/Sprite.h>

namespace Bonk {
	class Bonk;

	class State {
	public:
		State(Sprite* sprite) : display(sprite){ }
		virtual ~State();

		virtual void start(Bonk& game) = 0;
		virtual void update(uint _time, Bonk& game) = 0;
		virtual void draw() = 0;
		virtual void stop(){ }

	protected:
		Sprite* display;
		Bonk* game;
	};
};

#endif