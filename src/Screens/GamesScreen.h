#ifndef CHATTER_FIRMWARE_GAMESSCREEN_H
#define CHATTER_FIRMWARE_GAMESSCREEN_H

#include <vector>
#include <functional>
#include "../Interface/LVScreen.h"
#include "../AutoPop.h"
#include "../Elements/User.h"
#include "../Games/GameEngine/Game.h"

class GamesScreen : public LVScreen, private InputListener {
public:
	GamesScreen();

	void onStart() override;
	void onStop() override;

private:
	AutoPop apop;

	std::vector<User*> elements;

	struct GameInfo {
		const char* name;
		const char* icon;
		std::function<Game*(GamesScreen* gamesScreen)> launch;
		const char* splash = nullptr;
	};

	static const GameInfo Games[];

	void buttonPressed(uint i) override;

};


#endif //CHATTER_FIRMWARE_GAMESSCREEN_H
