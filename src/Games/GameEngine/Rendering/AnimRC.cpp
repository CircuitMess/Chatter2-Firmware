#include "AnimRC.h"

#include <utility>

//parentSprite as nullptr is safe as long as you pass the correct parent Sprite* in the push() function
AnimRC::AnimRC(File file) : gif(std::make_unique<GIFAnimatedSprite>(nullptr, file)){
	gif->setLoopMode(loopMode);
}

void AnimRC::setAnim(File file){
	gif->stop();
	gif.reset();
	gif = std::make_unique<GIFAnimatedSprite>(nullptr, file);
	gif->setLoopMode(loopMode);

	if(playing){
		gif->start();
	}
}

void AnimRC::start(){
	if(playing) return;

	gif->start();
	playing = true;
}

void AnimRC::stop(){
	if(!playing) return;

	gif->stop();
	playing = false;
}

void AnimRC::reset(){
	gif->reset();
}

void AnimRC::push(Sprite* parent, PixelDim pos, float rot) const{
	if(rot == 0){
		gif->push(parent, pos.x, pos.y);
	}else{
		gif->pushRotate(parent, pos.x, pos.y, rot);
	}
}

void AnimRC::setLoopDoneCallback(std::function<void(uint32_t)> cb){
	gif->setLoopDoneCallback(std::move(cb));
}

void AnimRC::setLoopMode(GIF::LoopMode loopMode){
	this->loopMode = loopMode;
	gif->setLoopMode(loopMode);
}
