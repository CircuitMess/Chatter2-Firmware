#include "AutoPop.h"
#include <Pins.hpp>
#include <Input/Input.h>

AutoPop::AutoPop(LVScreen* screen) : screen(screen){}

AutoPop::~AutoPop(){
	stop();
}

void AutoPop::start(){
	Input::getInstance()->addListener(this);
}

void AutoPop::stop(){
	Input::getInstance()->removeListener(this);
}

void AutoPop::buttonPressed(uint i){
	if(i != BTN_BACK) return;
	screen->pop();
}
