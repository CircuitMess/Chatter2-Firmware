#include "Score.h"

Score::Score(File icon) : icon(icon){
	go = std::make_shared<GameObject>(
			std::make_unique<SpriteRC>(PixelDim{ 28, 8 }),
			nullptr
	);

	rc = std::static_pointer_cast<SpriteRC>(go->getRenderComponent());
	rc->setLayer(10);
	sprite = rc->getSprite();

	sprite->setTextFont(0);
	sprite->setTextSize(0);
	sprite->setTextColor(TFT_WHITE);

	setScore(0);
}

void Score::setScore(uint16_t score){
	sprite->clear(TFT_TRANSPARENT);

	sprite->drawIcon(icon, 28 - 7, 0, 7, 7, 1, TFT_TRANSPARENT);
	sprite->setCursor(0, 0);
	sprite->printf("%3d", score);
}

std::shared_ptr<GameObject> Score::getGO() const{
	return go;
}
