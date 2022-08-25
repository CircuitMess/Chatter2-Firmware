#ifndef CIRCUITPET_FIRMWARE_GAMESYSTEM_H
#define CIRCUITPET_FIRMWARE_GAMESYSTEM_H

#include "GameObject.h"
#include <vector>
#include <set>

class Game;

class GameSystem {
public:
	virtual void update(uint32_t deltaMicros) = 0;

protected:
	GameSystem(const Game* game);

	std::set<std::shared_ptr<GameObject>> getObjects();

private:
	const Game* game;

};


#endif //CIRCUITPET_FIRMWARE_GAMESYSTEM_H
