#ifndef CHATTER_FIRMWARE_INPUTCHATTER_H
#define CHATTER_FIRMWARE_INPUTCHATTER_H

#include "InputLVGL.h"
#include <Input/InputListener.h>
#include <map>
#include <queue>

class InputChatter : public InputLVGL , public InputListener{
public:
	void read(lv_indev_drv_t* drv, lv_indev_data_t* data) override;

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

private:
	struct KeyState {
		lv_key_t key;
		bool pressed;
	};

	static std::map<uint8_t, std::vector<lv_key_t>> keyMap;
	std::queue<KeyState> btnQueue;

};


#endif //CHATTER_FIRMWARE_INPUTCHATTER_H
