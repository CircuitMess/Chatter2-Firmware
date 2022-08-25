#include "GameSystem.h"
#include "Game.h"

GameSystem::GameSystem(const Game* game) : game(game){ }

std::set<std::shared_ptr<GameObject>> GameSystem::getObjects(){
	return game->objects;
}