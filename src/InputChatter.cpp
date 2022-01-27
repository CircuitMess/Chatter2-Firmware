#include "InputChatter.h"
#include <Input/Input.h>
#include <Chatter.h>
#include <vector>

const std::map<uint8_t, lv_key_t> InputChatter::KeyMap = {
		{ BTN_LEFT,  LV_KEY_PREV },
		{ BTN_RIGHT, LV_KEY_NEXT },
		{ BTN_ENTER, LV_KEY_ENTER },
		{ BTN_BACK,  LV_KEY_ESC },
		{ BTN_R,     LV_KEY_DEL }
};


InputChatter::InputChatter() : InputLVGL(LV_INDEV_TYPE_KEYPAD){

}

void InputChatter::read(lv_indev_drv_t* drv, lv_indev_data_t* data){
	if(lastKey == (uint32_t)-1) return;

	auto key = KeyMap.find(lastKey);
	if(key == KeyMap.end()) return;

	data->key = key->second;
	data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

void InputChatter::buttonReleased(uint i){
	pressed = false;
	lastKey = i;
}

void InputChatter::buttonPressed(uint i){
	pressed = true;
	lastKey = i;
}



