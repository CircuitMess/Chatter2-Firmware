#include "Hearts.h"

Hearts::Hearts(File heart) : heart(heart){
	go = std::make_shared<GameObject>(
			std::make_unique<SpriteRC>(PixelDim{ 25, 6 }),
			nullptr
	);

	rc = std::static_pointer_cast<SpriteRC>(go->getRenderComponent());
	rc->setLayer(10);
	sprite = rc->getSprite();

	setLives(3);
}

void Hearts::setLives(uint8_t lives){
	sprite->clear(TFT_TRANSPARENT);

	for(int i = 0; i < lives; i++){
		sprite->drawIcon(heart, i*9, 0, 7, 6, 1, TFT_TRANSPARENT);
	}
}

std::shared_ptr<GameObject> Hearts::getGO() const{
	return go;
}
