#include "InputChatter.h"
#include <Input/Input.h>
#include <ByteBoi.h>
//#include <Chatter.h>
#include <vector>
#include <queue>

std::map<uint8_t, std::vector<lv_key_t>> keyMap = {{BTN_UP,   {LV_KEY_PREV, LV_KEY_UP,   LV_KEY_LEFT}},
												   {BTN_DOWN, {LV_KEY_NEXT, LV_KEY_DOWN, LV_KEY_RIGHT}},
												   {BTN_A,    {LV_KEY_ENTER}},
												   {BTN_B,    {LV_KEY_ESC}},
												   {BTN_C, {LV_KEY_DEL}}};
struct KeyStruct {
	lv_key_t key;
	bool pressed;
};
std::queue<KeyStruct> btnQueue;

void InputChatter::read(lv_indev_drv_t* drv, lv_indev_data_t* data){
	if(btnQueue.empty()) return;


	data->state = btnQueue.front().pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
	data->key = btnQueue.front().key;
	btnQueue.pop();

}


void InputChatter::buttonReleased(uint i){
	for(auto key: keyMap[i]){
		btnQueue.push({key, false});
	}
}

void InputChatter::buttonPressed(uint i){
	for(auto key: keyMap[i]){
		btnQueue.push({key, true});
	}
}

