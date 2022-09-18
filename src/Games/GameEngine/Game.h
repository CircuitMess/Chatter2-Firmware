#ifndef CIRCUITPET_FIRMWARE_GAME_H
#define CIRCUITPET_FIRMWARE_GAME_H

#include "ResourceManager.h"
#include "GameObject.h"
#include <Util/Task.h>
#include <set>
#include <vector>
#include "GameSystem.h"
#include "Collision/CollisionSystem.h"
#include "Rendering/RenderSystem.h"
#include "../../Interface/LVScreen.h"
#include "../../Screens/GamesScreen.h"
#include <Loop/LoopListener.h>
#include <Audio/ChirpSystem.h>

class GamesScreen;

class Game : private LoopListener {
friend GameSystem;
public:
	virtual ~Game();

	void load();
	bool isLoaded() const;

	void start();
	void stop();
	void pop();

	GamesScreen* getGamesScreen();

protected:
	Game(GamesScreen* gamesScreen, const char* root, std::vector<ResDescriptor> resources);

	virtual void onStart();
	virtual void onStop();
	virtual void onLoad();
	virtual void onLoop(float deltaTime);
	virtual void onRender(Sprite* canvas);

	File getFile(std::string path);

	void addObject(std::shared_ptr<GameObject> obj);
	void removeObject(std::shared_ptr<GameObject> obj);

	CollisionSystem collision;

	ChirpSystem Audio;

private:
	ResourceManager resMan;
	const std::vector<ResDescriptor> resources;

	bool loaded = false;
	Task loadTask;

	volatile bool popped = false;
	bool started = false;

	RenderSystem render;

	std::set<std::shared_ptr<GameObject>> objects;

	void loop(uint micros) final;
	void loadFunc();

	GamesScreen* gamesScreen;
};


#endif //CIRCUITPET_FIRMWARE_GAME_H
