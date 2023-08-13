#include "InputChatter.h"
#include <Input/Input.h>
#include <Chatter.h>
#include <vector>

std::map<uint8_t, lv_key_t> InputChatter::keyMap = {{BTN_UP,   LV_KEY_LEFT},
													{BTN_DOWN, LV_KEY_RIGHT},
													{BTN_A,    LV_KEY_ENTER},
													{BTN_B,    LV_KEY_ESC}};


InputChatter* InputChatter::instance = nullptr;

InputChatter::InputChatter() : InputLVGL(LV_INDEV_TYPE_ENCODER){
	instance = this;
}

void InputChatter::read(lv_indev_drv_t* drv, lv_indev_data_t* data){
	if(lastKey == (uint32_t)-1) return;

	data->key = keyMap[lastKey];
	data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

void InputChatter::buttonReleased(uint i){
	if(!keyMap.count(i)) return;

	pressed = false;
	lastKey = i;
}

void InputChatter::buttonPressed(uint i){
	if(!keyMap.count(i)) return;

	pressed = true;
	lastKey = i;
}

InputChatter* InputChatter::getInputInstance(){
	return instance;
}



