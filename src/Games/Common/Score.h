#ifndef CIRCUITPET_FIRMWARE_SCORE_H
#define CIRCUITPET_FIRMWARE_SCORE_H


#include "../GameEngine/GameObject.h"
#include "../GameEngine/Rendering/SpriteRC.h"

class Score {
public:
	Score(File icon);

	std::shared_ptr<GameObject> getGO() const;
	void setScore(uint16_t score);

private:
	File icon;

	std::shared_ptr<GameObject> go;
	std::shared_ptr<SpriteRC> rc;
	std::shared_ptr<Sprite> sprite;

};


#endif //CIRCUITPET_FIRMWARE_SCORE_H
