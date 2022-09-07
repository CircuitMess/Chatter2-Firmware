#ifndef BONK_TITLESTATE_H
#define BONK_TITLESTATE_H

#include "State.hpp"

namespace Bonk {
	class TitleState : public State {
	public:
		TitleState(Sprite* sprite);
		virtual ~TitleState();
		void update(uint _time, Bonk& game) override;
		void draw() override;
		void start(Bonk& game) override;
		void stop() override;
	private:
		static const char* titleMenu[3] PROGMEM;
		uint8_t titleCursor;
		bool blinkState;
		uint blinkMicros;
		static TitleState* instance;
	};
};

#endif