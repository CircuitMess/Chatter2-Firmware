#ifndef CHATTER_FIRMWARE_GAMESSCREEN_H
#define CHATTER_FIRMWARE_GAMESSCREEN_H

#include <vector>
#include <functional>
#include "../Interface/LVScreen.h"
#include "../AutoPop.h"
#include "../Elements/User.h"
#include "../Games/GameEngine/Game.h"

class GamesScreen : public LVScreen{
public:
	GamesScreen();

	void onStart() override;
	void onStop() override;

private:
	AutoPop apop;

	std::vector<User*> elements;

	struct GameInfo {
		const char* name;
		std::function<Game*(GamesScreen* gamesScreen)> launch;
	};

	static const GameInfo Games[];

};


#endif //CHATTER_FIRMWARE_GAMESSCREEN_H
