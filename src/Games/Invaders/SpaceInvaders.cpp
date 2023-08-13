#include "SpaceInvaders.h"
#include "sprites.hpp"
#include <Chatter.h>

uint drawTime = 0;
SpaceInvaders::SpaceInvaders* SpaceInvaders::SpaceInvaders::instance = nullptr;
SpaceInvaders::SpaceInvaders::SpaceInvaders(GamesScreen* gamesScreen) : Game(gamesScreen, "/", {}), baseSprite(Chatter.getDisplay()->getBaseSprite()){
	buttons = Input::getInstance();
	instance = this;
	randomSeed(millis()*micros());
	starsSetup();
	gamestatus = "title";

	hs.begin("Invaders");
}
void SpaceInvaders::SpaceInvaders::onStart()
{
	prevGamestatus = "";
	draw();
}
void SpaceInvaders::SpaceInvaders::onStop()
{
	clearButtonCallbacks();
	if(input){
		input->stop();
		delete input;
		input = nullptr;
	}
}

void SpaceInvaders::SpaceInvaders::draw(){
	if (gamestatus == "title") {
		showtitle();
	}
	if (gamestatus == "running") { // game running loop
		baseSprite->clear(TFT_BLACK);
		for(int i = 0; i < STAR_COUNT; i++)
		{
			// Remove the star from the screen by changing its pixel to the background color.
			baseSprite->drawPixel(stars[i].x, stars[i].y, BACKGROUND_COLOR);
			// Draw the star at its new coordinate.
			baseSprite->fillRect(stars[i].x, stars[i].y, 2, 2, STAR_COLOR);

		}
		drawplayership(); // draw player ship
		drawplayershot(); // move + draw player shoot
		drawinvaders(); // draw invaders
		drawInvaderShot(); // invaders shoot
		drawbunkers(); // draw bunkers & check collission with player shot
		drawsaucer(); // draw saucer & remove if hit
		showscore(); // show lives, score, level
	}
	if (gamestatus == "gameover") { // game over
		baseSprite->clear(TFT_BLACK);
		drawplayership(); // draw player ship
		drawplayershot(); // move + draw player shoot
		drawinvaders(); // draw invaders
		drawInvaderShot(); // invaders shoot
		drawbunkers(); // draw bunkers & check collission with player shot
		drawsaucer(); // draw saucer & remove if hit
		baseSprite->setTextColor(TFT_RED);
		baseSprite->setTextSize(2);
		baseSprite->setFont(&fonts::Font0);
		baseSprite->drawRect(14, 41, 134, 38, TFT_WHITE);
		baseSprite->drawRect(13, 40, 136, 40, TFT_WHITE);
		baseSprite->fillRect(15, 42, 132, 36, TFT_BLACK);
		baseSprite->setTextDatum(textdatum_t::middle_center);
		baseSprite->drawString("GAME OVER", baseSprite->width() / 2, baseSprite->height() / 2);
		baseSprite->setTextDatum(textdatum_t::bottom_center);

	}
	if(gamestatus == "paused")
	{
		baseSprite->clear(TFT_BLACK);
		baseSprite->setTextColor(TFT_RED);
		baseSprite->setFont(&fonts::Font2);
		baseSprite->setTextSize(2);
		baseSprite->drawString("Paused", baseSprite->width()/2, baseSprite->height()/2 - 10);
		baseSprite->setFreeFont(&TomThumb);
		baseSprite->setCursor(30, baseSprite->height() - 30);
		baseSprite->print("ENTER: Resume");
		baseSprite->setCursor(45, baseSprite->height() - 10);
		baseSprite->print("BACK: Quit");
	}
	if(gamestatus == "eraseData")
	{
		eraseDataDraw();
	}
	if(gamestatus == "dataDisplay")
	{
		dataDisplay();
	}
	if(gamestatus == "enterInitials")
	{
		enterInitialsDraw();
	}
}
void SpaceInvaders::SpaceInvaders::onLoop(float dt)
{
	if(gamestatus != prevGamestatus)
	{
		screenChange = 1;
		prevGamestatus = gamestatus;
	}
	else
	{
		screenChange = 0;
	}
	
	if (gamestatus == "title") {
		if(screenChange)
		{
			titleSetup();
		}
		if(millis() - blinkMillis >= 250)
		{
			blinkMillis = millis();
			blinkState = !blinkState;
		}
		for(int i = 0; i < STAR_COUNT; i++)
		{
			// Update the position of the star.
			stars[i].update(dt);

			// If the star's Y position is off the screen after the update.
			if(stars[i].y >= baseSprite->height())
			{
				// Randomize its position.
				stars[i].randomize(0, baseSprite->width(), 0, baseSprite->height(), STAR_SPEED_MIN, STAR_SPEED_MAX);
				// Set its Y position back to the top.
				stars[i].y = 0;
			}
		}
	}
	if (gamestatus == "newgame") { newgame(); } // new game

	if (gamestatus == "newlevel") { newlevel(); } // new level

	if (gamestatus == "running") { // game running loop
		if(screenChange)
		{
			setButtonsCallbacks();
		}
		for(int i = 0; i < STAR_COUNT; i++)
		{
			// Update the position of the star.
			stars[i].update(dt);

			// If the star's Y position is off the screen after the update.
			if(stars[i].y >= baseSprite->height())
			{
				// Randomize its position.
				stars[i].randomize(0, baseSprite->width(), 0, baseSprite->height(), STAR_SPEED_MIN, STAR_SPEED_MAX);
				// Set its Y position back to the top.
				stars[i].y = 0;
			}
		}
		invaderlogic(dt); // invader logic
		updateInvaderShot(dt);
		updatePlayerShot(dt);
		nextlevelcheck(dt); // next level?
		saucerappears(dt); // saucer appears?
		movesaucer(dt); // move saucer
	}
	if (gamestatus == "gameover") { // game over
		if(screenChange){
			Audio.play({{ 400, 300, 200 },
						{ 0,   0,   50 },
						{ 300, 200, 200 },
						{ 0,   0,   50 },
						{ 200, 50,  400 }});

			delay(100);
			clearButtonCallbacks();
			buttons->setBtnPressCallback(BTN_A, [](){
				instance->gamestatus = "enterInitials";
			});
			buttons->setBtnPressCallback(BTN_B, [](){
				instance->gamestatus = "enterInitials";
			});
		}
	}
	if(gamestatus == "paused")
	{
		if(screenChange){
			clearButtonCallbacks();
			buttons->setBtnPressCallback(BTN_A, [](){
				instance->gamestatus = "running";
				instance->setButtonsCallbacks();
			});
			buttons->setBtnPressCallback(BTN_B, [](){
				instance->gamestatus = "title";
			});
		}
	}
	if(gamestatus == "eraseData")
	{
		if(screenChange){
			eraseDataSetup();
		}
		eraseDataUpdate();
	}
	if(gamestatus == "dataDisplay")
	{
		if(screenChange){
			dataDisplaySetup();
		}
	}
	if(gamestatus == "enterInitials")
	{
		if(screenChange){
			enterInitialsSetup();
		}
		enterInitialsUpdate();
	}
	draw();
}
void SpaceInvaders::SpaceInvaders::starsSetup()
{
	// Loop through each star.
	for(int i = 0; i < STAR_COUNT; i++)
	{
		// Randomize its position && speed.
		stars[i].randomize(0, baseSprite->width(), 0, baseSprite->height(), STAR_SPEED_MIN, STAR_SPEED_MAX);
	}
} 
void SpaceInvaders::SpaceInvaders::drawBitmap(int16_t x, int16_t y, const byte *bitmap, uint16_t color, uint8_t scale) {
	uint16_t w = *(bitmap++);
	uint16_t h = *(bitmap++);
	baseSprite->drawBitmap(x, y, bitmap, w, h, color, scale);
}

//ported nonstandard
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::newgame() {
	score = 0;
	lives = 3;
	gamelevel = 0;
	shipx = baseSprite->width() / 2 - 4;
	shipy = baseSprite->height() - 14;
	shotx = -1;
	shoty = -1;
	deadcounter = -1;
	saucers = -1;
	invadershots = 0;

	starsSetup();
	for (int i = 0; i < 4; i++) {
		invadershotx[i] = -1;
		invadershoty[i] = -1;
	}
	gamestatus = "newlevel";
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::newlevel() {
	invaderanz = invadersRows*invadersColumns;
	invaderctr = invadersRows*invadersColumns - 1;
	invaderxr = 1;
	invaderyr = 1;
	checkdir = 0;
	nextxdir = 4;
	nextydir = 0;
	yeahtimer = 0;
	invadershottimer = 120;
	saucertimer = 14;
	int down = gamelevel * 2;
	if (gamelevel > 8) { down = 20; }
	for (int i = 0; i < invadersColumns*invadersRows; i++) {
		invaderx[i] = 10 + i%invadersColumns * 16;
		invadery[i] = 14 + 12*int(i/invadersColumns) + down;
		// invadery[i] = 14 + down;
		// invadery[i + 6] = 13*2 + down;
		// invadery[i + 12] = 19*2 + down;
		// invadery[i + 18] = 25*2 + down;
		// invadery[i + 24] = 31*2 + down;
		switch (int(i/invadersColumns))
		{
		case 0:
			invaders[i] = 4;
			break;
		case 1:
			invaders[i] = 2;
			break;
		case 2:
			invaders[i] = 2;
			break;
		case 3:
			invaders[i] = 0;
			break;
		case 4:
			invaders[i] = 0;
			break;

		default:
			break;
		}
		invaderframe[i] = 0;
	}
	for (int & bunker : bunkers) {
		bunker = 0;
		if (gamelevel > 5) { bunker = -1; }
	}
	gamestatus = "running";
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::showscore() {
	if (infoshow == 1 && saucers == -1) {
		if (lives > 1) { drawBitmap(0, 0, invaderz_playership[0], TFT_WHITE, 2); }
		if (lives > 2) { drawBitmap(18, 0, invaderz_playership[0], TFT_WHITE, 2); }
		baseSprite->setTextColor(TFT_WHITE);
		baseSprite->setFreeFont(&TomThumb);
		baseSprite->setTextSize(2);
		baseSprite->setCursor(117 - 4 * (score > 9) - 4 * (score > 99) - 4 * (score > 999), 12);
		baseSprite->print(score);
		baseSprite->setCursor(144, 12);
		baseSprite->print(gamelevel + 1);
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::nextlevelcheck(float dt) {
	// increment timer after all invaders killed
	if (invaderanz == 0) {
		yeahtimer += dt;
		if (yeahtimer >= 2) {
			gamelevel++;
			gamestatus = "newlevel";
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::handledeath() {
	deadcounter--;
	if (deadcounter == 0) {
		deadcounter = -1;
		lives--;
		shipx = baseSprite->width() / 2 - 4;
		if (lives == 0) { gamestatus = "gameover"; }
	}
}


//ported specific
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::clearButtonCallbacks()
{
	for(auto i : { BTN_A, BTN_B, BTN_LEFT, BTN_RIGHT })
	{
		buttons->setBtnReleaseCallback(i, nullptr);
		buttons->setBtnPressCallback(i, nullptr);
		buttons->setButtonHeldRepeatCallback(i, 0, nullptr);
		buttons->setButtonHeldCallback(i, 0, nullptr);
	}
}
void SpaceInvaders::SpaceInvaders::setButtonsCallbacks() {
	clearButtonCallbacks();
	buttons->setButtonHeldRepeatCallback(BTN_LEFT, 10, [](uint){
		if (instance->shipx > 0 && instance->deadcounter == -1) {
			instance->shipx-=1.5f;
		}
	});
	buttons->setButtonHeldRepeatCallback(BTN_RIGHT, 10, [](uint){
		if (instance->shipx < 143 && instance->deadcounter == -1) {
			instance->shipx+=1.5f;
		}
	});
	buttons->setBtnPressCallback(BTN_A, [](){
		if(instance->shotx == -1 && instance->deadcounter == -1){
			instance->Audio.play({{450, 300, 100}});
			instance->shotx = instance->shipx + 6;
			instance->shoty = instance->shipy - 2;
		}
	});
	buttons->setBtnPressCallback(BTN_B, [](){
		instance->gamestatus = "paused";
	});
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::drawplayership() {
	if (deadcounter == -1) {
		drawBitmap(shipx, shipy, invaderz_playership[0], TFT_WHITE, 2);
	}
	else {
		drawBitmap(shipx, shipy, invaderz_playership[1 + invadershotframe], TFT_YELLOW, 2);
		handledeath();
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::drawplayershot() {
	if (shotx != -1) {
		baseSprite->drawLine(shotx, shoty, shotx, shoty + 6, TFT_YELLOW);
		baseSprite->drawLine(shotx+1, shoty, shotx+1, shoty + 6, TFT_YELLOW);
	}
}
void SpaceInvaders::SpaceInvaders::updatePlayerShot(float dt)
{
	if (shotx != -1) {
		shoty = shoty - 160.0f * dt;
		if (shoty < 0) {
			shotx = -1;
			shoty = -1;
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::invaderlogic(float dt) {
	updateInvadersTime += dt;
	if(updateInvadersTime < updateInvadersInterval) return;
	updateInvadersTime -= updateInvadersInterval;

	// increment invader counter
	if (invaderanz > 0) {
		checkdir = 0;
		if(invaders[invaderctr] == -1)
		{
			while (invaders[invaderctr] == -1){
				invaderctr++;
				if (invaderctr >= invadersColumns*invadersRows) {
					invaderctr = 0;
					checkdir = 1;
				}

			}
		}
		else
		{
			invaderctr++;
			if (invaderctr >= invadersColumns*invadersRows) {
				invaderctr = 0;
				checkdir = 1;
			}
		}
		

		// change direction?
		if (checkdir == 1) {
			if (nextydir != 0) {
				invaderxr = 0;
				invaderyr = 2;
			}
			else {
				invaderxr = nextxdir;
				invaderyr = 0;
			}
			checkdir = 0;
		}

		// change invader position
		invaderx[invaderctr] = invaderx[invaderctr] + invaderxr*3;
		invadery[invaderctr] = invadery[invaderctr] + invaderyr*3;

		// determine bunker removal if invaders are too low
		if (invadery[invaderctr] > 80) {
			for (int i = 0; i < 4; i++) {
				bunkers[i] = -1;
			}
		}

		// determine game over if invaders reach bottom
		if (invadery[invaderctr] >= shipy - 14) {
			gamestatus = "gameover";
		}

		// determine screen border hit -> go down, then change direction
		if (invaderx[invaderctr] > baseSprite->width() - 20 && invaderxr > 0) {
			nextxdir = -4;
			nextydir = 4;
		}
		if (invaderx[invaderctr] < 6 && invaderxr < 0) {
			nextxdir = 4;
			nextydir = 4;
		}
		if (invaderyr != 0) { nextydir = 0; }

		//change invader shape
		invaderframe[invaderctr] = ++invaderframe[invaderctr] % 2;

		// remove killed invader
		if (invaders[invaderctr] == 6) {
			invaders[invaderctr] = -1;
			invaderanz--;
		}

		// release invadershoot
		if (invadershottimer <= 0 && invadershots < gamelevel + 1 && invadershots < 4 && invadery[invaderctr] < 80) {
			invadershottimer = 120 - gamelevel * 10;
			invadershots++;
			int flag = 0;
			for (int u = 0; u < 4; u++) {
				if (flag == 0 && invadershotx[u] == -1) {
					uint8_t x = random(0, invaderanz);
					for(int8_t i = x; i >= 0; i--)
					{
						if(invaders[i] == -1)
						{
							x++;
						}
					}
					invadershotx[u] = invaderx[x] + 2;
					invadershoty[u] = invadery[x];
					flag = 1;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::drawinvaders() {
	infoshow = 1;
	for (int i = 0; i < 40; i++) {
		if (invaders[i] != -1) {
			if (invaders[i] == 0) drawBitmap(invaderx[i], invadery[i], invaderz_invader[invaders[i] + invaderframe[i]], TFT_ORANGE, 2);
			if (invaders[i] == 2) drawBitmap(invaderx[i], invadery[i], invaderz_invader[invaders[i] + invaderframe[i]], TFT_PINK, 2);
			if (invaders[i] == 4) drawBitmap(invaderx[i], invadery[i], invaderz_invader[invaders[i] + invaderframe[i]], TFT_BLUE, 2);
			if (invaders[i] == 6) drawBitmap(invaderx[i], invadery[i], invaderz_invader[invaders[i] + invaderframe[i]], TFT_YELLOW, 2);
			
			if (invadery[i] < 5) {
				infoshow = 0;
			}
		}

		// determine collission: invader & player shoot
		checkl = invaderx[i];
		checkr = invaderx[i] + 12;
		checkt = invadery[i];
		checkb = invadery[i] + 8;
		if (invaders[i] == 4) {
			checkl++;
			checkr--;
		}
		if (invaders[i] != -1 && invaders[i] != 6 && shotx >= checkl && shotx <= checkr && shoty + 2 >= checkt && shoty <= checkb) {
			score = score + invaders[i] * 10 + 10;
			invaders[i] = 6;
			shotx = -1;
			shoty = -1;
			// destroyed->note(10, 0.05);
			Audio.play({{100, 100, 50}});

			// invaderDestroyed->play();
		}

	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::drawInvaderShot()
{
	for (int i = 0; i < 4; i++) {
		if (invadershotx[i] != -1) {
			drawBitmap(invadershotx[i], invadershoty[i], invaderz_bomb[invadershotframe], TFT_RED, 2);
		}
	}
}

void SpaceInvaders::SpaceInvaders::updateInvaderShot(float dt) {
	// handle invadershoot timer & framecounter
	invadershottimer--;
	if(invadershotframe == pastInvaderShotFrame)
		invadershotframe = !invadershotframe;
	else
		pastInvaderShotFrame = !pastInvaderShotFrame;
	// move invadershots, draw & check collission
	for (int i = 0; i < 4; i++) {
		if (invadershotx[i] != -1) {
			invadershoty[i] = invadershoty[i] + 60.0f * dt;

			// check collission: invadershot & bunker
			for (int u = 0; u < 4; u++) {
				checkl = 22 + u * 36;
				checkr = 22 + u * 36 + 14;
				checkt = 90;
				checkb = 100;
				if (bunkers[u] != -1 && invadershotx[i] + 1 >= checkl && invadershotx[i] <= checkr && invadershoty[i] + 3 >= checkt && invadershoty[i] <= checkb) {
					bunkers[u]++;
					if (bunkers[u] > 4) { bunkers[u] = -1; }
					invadershotx[i] = -1;
					invadershoty[i] = -1;
					invadershots--;
				}

			}

			// check collission: invadershot & player
			checkl = shipx;
			checkr = shipx + 12;
			checkt = shipy;
			checkb = shipy + 6;
			if (deadcounter == -1 && invadershotx[i] + 1 >= checkl && invadershotx[i] <= checkr && invadershoty[i] + 3 >= checkt && invadershoty[i] <= checkb) {
				deadcounter = 60;
				// destroyed->note(10, 0.05);
				Audio.play({{100, 80, 150}});
				// playerDestroyed->play();
			}

			//check collission: invadershot & playershoot
			checkl = invadershotx[i];
			checkr = invadershotx[i] + 1;
			checkt = invadershoty[i];
			checkb = invadershoty[i] + 3;
			if (shotx >= checkl && shotx <= checkr && shoty + 2 >= checkt && shoty <= checkb) {
				shotx = -1;
				shoty = -1;
				invadershotx[i] = -1;
				invadershoty[i] = -1;
				invadershots--;
			}

			// invadershoot on bottom off screen?
			if (invadershoty[i] > baseSprite->height()) {
				invadershotx[i] = -1;
				invadershoty[i] = -1;
				invadershots--;
			}
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::drawbunkers() {
	for (int i = 0; i < 4; i++) {
		checkl = 22 + i * 36;
		checkr = 22 + i * 36 + 14;
		checkt = 90;
		checkb = 100;
		if (bunkers[i] != -1 && shotx >= checkl && shotx <= checkr && shoty + 2 >= checkt && shoty <= checkb) {
			bunkers[i]++;
			shotx = -1;
			shoty = -1;
			if (bunkers[i] > 4) { bunkers[i] = -1; }
		}

		if (bunkers[i] != -1) {
			drawBitmap(22 + i * 36, 90, invaderz_bunker[bunkers[i]], TFT_GREEN, 2);
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::saucerappears(float dt) {
	saucertimer -= dt;
	if (saucertimer <= 0) {
		saucertimer = 14;
		if (infoshow == 1 && saucers == -1) {
			// mainMusic->pause();
			// removeTrack(mainMusic);
			// addTrack(ufoSound);
			// ufoSound->setSpeed(2);
			// ufoSound->setRepeat(1);
			// ufoSound->play();
			saucers = 0;
			int i = random(2);
			if (i == 0) {
				saucerx = 0;
				saucerdir = 1;
			}
			else {
				saucerx = baseSprite->width() + 18;
				saucerdir = -1;
			}
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::movesaucer(float dt) {
	if (saucers == 0) {
		saucerx = saucerx + ((float) saucerdir) * 50.0f * dt;
		if (saucerx <= 0 || saucerx >= baseSprite->width() + 18) {
	// removeTrack(ufoSound);
	// mainMusic->resume();
	// addTrack(mainMusic);
			saucers = -1;
		}

		// check collission: player shot & saucer  
		checkl = saucerx;
		checkr = saucerx + 20;
		checkt = 0;
		checkb = 6;
		if (shotx >= checkl && shotx <= checkr && shoty + 2 >= checkt && shoty <= checkb) {
			score += 100;
			saucers = 1;
			shotx = -1;
			shoty = -1;
			saucerwait = 30;
			Audio.play({{100, 120, 50}});
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::drawsaucer() {
	if (saucers != -1) {
		drawBitmap(saucerx, 0, invaderz_saucer[saucers], TFT_RED, 2);
		if (saucers == 1) {
			saucerwait--;
			if (saucerwait <= 0) {
		// removeTrack(ufoSound);
		// addTrack(mainMusic);
		// mainMusic->play();
		// mainMusic->setRepeat(1);
				saucers = -1;
			}
		}
	}
}


//----------------------------------------------------------------------------
void SpaceInvaders::SpaceInvaders::eraseDataSetup()
{
	elapsedMillis = millis();
	blinkState = 1;
	clearButtonCallbacks();
	buttons->setBtnPressCallback(BTN_B, [](){
		instance->gamestatus = "dataDisplay";

	});
	buttons->setBtnPressCallback(BTN_A, [](){
		instance->hs.clear();
		instance->gamestatus = "title";

	});
}
void SpaceInvaders::SpaceInvaders::eraseDataDraw()
{
	baseSprite->clear(TFT_BLACK);
	baseSprite->setFont(&fonts::Font2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString("ARE YOU SURE?", baseSprite->width() / 2, 17);
	baseSprite->drawString("This cannot be reverted!", baseSprite->width() / 2, 37);
//	baseSprite->drawString("", screen.getWidth() / 2, 53);

	baseSprite->drawString("BACK: Cancel", baseSprite->width() / 2, 105);

	baseSprite->setCursor(25, 81);
	baseSprite->print("ENTER:");

	if (blinkState){
		baseSprite->drawRect(72, 64, 30*2, 9*2, TFT_RED);
		baseSprite->drawRect(72, 64, 30*2, 9*2, TFT_RED);
		baseSprite->setTextColor(TFT_RED);
		baseSprite->setCursor(79, 81);
		baseSprite->print("DELETE");
	}
	else {
		baseSprite->fillRect(72, 64, 30*2, 9*2, TFT_RED);
		baseSprite->setTextColor(TFT_WHITE);
		baseSprite->setCursor(79, 81);
		baseSprite->print("DELETE");
	}
}
void SpaceInvaders::SpaceInvaders::eraseDataUpdate()
{
	if (millis() - elapsedMillis >= 350) {
		elapsedMillis = millis();
		blinkState = !blinkState;
	}
}

void SpaceInvaders::SpaceInvaders::dataDisplaySetup()
{
	clearButtonCallbacks();
	buttons->setBtnPressCallback(BTN_UP, [](){
		instance->gamestatus = "eraseData";
	});
	buttons->setBtnPressCallback(BTN_A, [](){
		instance->Audio.play(Sound { Chirp { 400, 350, 50 }});
		instance->gamestatus = "title";
	});
	buttons->setBtnPressCallback(BTN_B, [](){
		instance->Audio.play(Sound { Chirp { 400, 350, 50 }});
		instance->gamestatus = "title";
	});
}
void SpaceInvaders::SpaceInvaders::dataDisplay()
{
	baseSprite->clear(TFT_BLACK);
	baseSprite->setTextSize(1);
	baseSprite->setFont(&fonts::Font2);
	baseSprite->setTextColor(TFT_RED);
	baseSprite->drawString("HIGHSCORES", baseSprite->width() / 2, 15);
	baseSprite->setCursor(3, 110);
	for (int i = 1; i < 6;i++)
	{
		baseSprite->setCursor(22, 16 + i * 16);
		if(i <= hs.count())
		{
			baseSprite->printf("%d.   %.3s    %04d", i, hs.get(i - 1).name, hs.get(i - 1).score);
		}
		else
		{
			baseSprite->printf("%d.    ---   ----", i);
		}
	}
	baseSprite->drawString("Press LEFT to erase", baseSprite->width() / 2, 117);
}

void SpaceInvaders::SpaceInvaders::showtitle() {
	baseSprite->clear(TFT_BLACK);
	for(int i = 0; i < STAR_COUNT; i++)
	{
		// Remove the star from the screen by changing its pixel to the background color.
		baseSprite->drawPixel(stars[i].x, stars[i].y, BACKGROUND_COLOR);
		// Draw the star at its new coordinate.
		baseSprite->fillRect(stars[i].x, stars[i].y, 2, 2, STAR_COLOR);
	}
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawIcon(titleLogo, (baseSprite->width() - 73*2) / 2, 7, 73, 21, 2, TFT_WHITE);
	baseSprite->setTextColor(TFT_RED);
	baseSprite->setFreeFont(&TomThumb);
	baseSprite->setTextSize(2);
	baseSprite->setTextDatum(textdatum_t::bottom_center);
	baseSprite->drawString("START", baseSprite->width()/2 + 5, 75);
	baseSprite->drawString("HIGHSCORES", baseSprite->width()/2 + 5, 95);
	baseSprite->drawString("QUIT", baseSprite->width()/2 + 5, 115);
	if(blinkState)
	{
		baseSprite->drawRect((baseSprite->width() - 98) / 2, 60 + cursor * 20, 98, 16, TFT_RED);
		baseSprite->drawRect((baseSprite->width() - 100) / 2, 59 + cursor * 20, 100, 18, TFT_RED);
	}
}
void SpaceInvaders::SpaceInvaders::titleSetup()
{
	cursor = 0;
	blinkMillis = millis();
	blinkState = 0;
	clearButtonCallbacks();

	buttons->setBtnPressCallback(BTN_UP, [](){
		if(instance->cursor > 0)
		{
			instance->cursor--;
			instance->blinkMillis = millis();
			instance->blinkState = 1;
			instance->Audio.play({{500, 500, 50}});
		}
	});
	buttons->setBtnPressCallback(BTN_DOWN, [](){
		if(instance->cursor < 2)
		{
			instance->cursor++;
			instance->blinkMillis = millis();
			instance->blinkState = 1;
			instance->Audio.play({{500, 500, 50}});
		}
	});
	
	buttons->setBtnPressCallback(BTN_A, [](){
		switch (instance->cursor)
		{
			case 0:
				instance->Audio.play({{500, 700, 50}});
				instance->gamestatus = "newgame";
				break;
			case 1:
				instance->Audio.play({{500, 700, 50}});
				instance->gamestatus = "dataDisplay";
				break;
			case 2:
				instance->Audio.play(Sound { Chirp { 400, 350, 50 }});
				instance->pop();
				return;
		}
	});

	buttons->setBtnPressCallback(BTN_B, [](){
		instance->Audio.play(Sound { Chirp { 400, 350, 50 }});
		instance->pop();
	});
}

void SpaceInvaders::SpaceInvaders::enterInitialsSetup()
{
	tempScore = hs.get(0).score;
	name = "AAA";
	charCursor = 0;
	previous = "";
	elapsedMillis = millis();
	hiscoreMillis = millis();
	blinkState = 1;
	hiscoreBlink = 0;
	clearButtonCallbacks();

	if(!input){
		input = new TextInput();
		input->setCallbacks(
				[](){
					if(instance->charCursor < 2){
						instance->charCursor++;
					}

					instance->blinkState = 1;
					instance->elapsedMillis = millis();
				},
				[](){
					if(instance->charCursor > 0){
						instance->charCursor--;
					}

					instance->blinkState = 1;
					instance->elapsedMillis = millis();
				},
				[](char c){
					instance->blinkState = 1;
					instance->elapsedMillis = millis();
					instance->name[instance->charCursor] = toUpperCase(c);
				}
		);
		input->start();
	}

	buttons->setBtnPressCallback(BTN_A, [](){
		Highscore::Score newScore;
		strcpy(newScore.name, instance->name.c_str());
		newScore.score = instance->score;
		instance->hs.add(newScore);
		instance->gamestatus = "dataDisplay";

		instance->input->stop();
		delete instance->input;
		instance->input = nullptr;

		instance->Audio.play({{500, 700, 50}});
	});
}
void SpaceInvaders::SpaceInvaders::enterInitialsUpdate() {
  
    if (millis() - elapsedMillis >= 350) //cursor blinking routine
	{
		elapsedMillis = millis();
		blinkState = !blinkState;
	}
    if(millis()-hiscoreMillis >= 1000)
    {
      hiscoreMillis = millis();
      hiscoreBlink = !hiscoreBlink;
    }
    previous = name;

    if (previous != name)
    {
      blinkState = 1;
      elapsedMillis = millis();
    }
}
void SpaceInvaders::SpaceInvaders::enterInitialsDraw() {
	baseSprite->clear(TFT_BLACK);
    baseSprite->setFont(&fonts::Font2);
    baseSprite->setTextColor(TFT_WHITE);
    baseSprite->setTextSize(1);
    baseSprite->drawString("ENTER NAME", baseSprite->width() / 2, 16);

    if(hiscoreBlink && score > tempScore)
    	baseSprite->drawString("NEW HIGH!", baseSprite->width() / 2, 80);
    else{
    	baseSprite->setCursor(39, 80);
    	baseSprite->printf("SCORE: %04d", score);
    }

    baseSprite->setCursor(66, 55);
    baseSprite->print(name[0]);
	baseSprite->setCursor(81, 55);
    baseSprite->print(name[1]);
	baseSprite->setCursor(96, 55);
    baseSprite->print(name[2]);
    // baseSprite->drawRect(30, 38, 100, 20, TFT_WHITE);
	if(blinkState){
		baseSprite->drawFastHLine(63 + 15*charCursor, 54, 12, TFT_WHITE);
	}
}
