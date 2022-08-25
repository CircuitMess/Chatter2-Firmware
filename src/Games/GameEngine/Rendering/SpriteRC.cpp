
#include "SpriteRC.h"

SpriteRC::SpriteRC(PixelDim dim) : sprite(std::make_shared<Sprite>((Sprite*) nullptr, dim.x, dim.y)){
	static_cast<TFT_eSprite*>(sprite.get())->setSwapBytes(false); //has to be called since it's not inherited from baseSprite or any other sprite
}

std::shared_ptr<Sprite> SpriteRC::getSprite() const{
	return sprite;
}

void SpriteRC::push(Sprite* parent, PixelDim pos, float rot) const{
	if(rot == 0){
		sprite->push(parent, pos.x, pos.y);
	}else{
		sprite->pushRotateZoomWithAA(parent, pos.x + sprite->width() / 2, pos.y + sprite->height() / 2, rot, 1, 1, TFT_TRANSPARENT);
	}
}
