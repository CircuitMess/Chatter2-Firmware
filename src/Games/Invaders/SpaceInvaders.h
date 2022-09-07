#ifndef INVADERZ_H
#define INVADERZ_H
#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include <Loop/LoopManager.h>
#include "Star.h"
#include <Support/Context.h>
#include <UI/Image.h>
#include "../GameEngine/Highscore.h"
#include "../GameEngine/Game.h"

namespace SpaceInvaders
{
	
constexpr int STAR_COUNT = 30;            // Number of stars on the screen. Arduino UNO maxes out around 250.
constexpr int BACKGROUND_COLOR = 0x0000;   // Background color in hex. 0x0000 is black.
constexpr int STAR_SPEED_MIN = 10;          // Minimum movement in pixels per update. (value is inclusive)
constexpr int STAR_SPEED_MAX = 40;         // Maximum movement in pixels per update. (value is inclusive)
constexpr int STAR_COLOR = 0xffff;  

class SpaceInvaders: public Game {
public:
	SpaceInvaders(GamesScreen* gamesScreen);
	void onStart() override;
	void onLoop(float) override;
	void onStop() override;

private:
	void draw();

	Input* buttons;
	Sprite *baseSprite;
	// StaticJsonDocument<8000> jb;
	String gamestatus;
	int score;
	int lives;
	int gamelevel;
	float shipx;
	int shipy;
	int invaderx[40];
	int invadery[40];
	int invaders[40];
	int invaderframe[40];
	int invaderanz;
	int invaderctr;
	int checkdir;
	int nextxdir;
	int nextydir;
	int invaderxr;
	int invaderyr;
	int invadershotx[4];
	float invadershoty[4];
	bool invadershotframe = 0;
	bool pastInvaderShotFrame = 0;
	int invadershots = 0;
	int invadershottimer;
	int bunkers[4];
	int shotx;
	float shoty;
	int checkl;
	int checkr;
	int checkt;
	int checkb;
	float yeahtimer;
	int infoshow;
	int deadcounter;
	float saucerx;
	int saucerdir;
	int saucers;
	float saucertimer;
	int saucerwait;
	uint32_t blinkMillis = millis();
	bool blinkState = 0;
	unsigned long elapsedMillis = millis();
	uint16_t hiscoresSize = 0;
	char nameArray[6][4];
	uint16_t scoreArray[6];
	uint8_t charCursor = 0;
	String previous = "";
	uint32_t hiscoreMillis = millis();
	bool hiscoreBlink = 0;
	char *highscoresPath;
	bool savePresent = 0;
	uint16_t tempScore = 0;
	String name = "";
	uint8_t cursor = 0;
	Star stars[STAR_COUNT];
	String prevGamestatus = gamestatus;
	bool screenChange = 0;
	static SpaceInvaders* instance;
	const uint8_t invadersColumns = 8;
	const uint8_t invadersRows = 5;

	Highscore hs;

	float updateInvadersTime = 0;
	static constexpr float updateInvadersInterval = .05f;

	void starsSetup();
	void drawBitmap(int16_t x, int16_t y, const byte *bitmap, uint16_t color, uint8_t scale);
	void newgame();
	void newlevel();
	void showscore();
	void nextlevelcheck(float dt);
	void handledeath();
	void clearButtonCallbacks();
	void setButtonsCallbacks();
	void drawplayership();
	void drawplayershot();
	void updatePlayerShot(float dt);
	void invaderlogic(float dt);
	void drawinvaders();
	void drawInvaderShot();
	void updateInvaderShot(float dt);
	void drawbunkers();
	void saucerappears(float dt);
	void movesaucer(float dt);
	void drawsaucer();
	void eraseDataSetup();
	void eraseDataDraw();
	void eraseDataUpdate();
	void dataDisplaySetup();
	void dataDisplay();
	void showtitle();
	void titleSetup();
	void enterInitialsSetup();
	void enterInitialsDraw();
	void enterInitialsUpdate();

};
};
#endif