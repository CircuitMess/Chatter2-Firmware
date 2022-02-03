#ifndef CHATTER_FIRMWARE_INPUTCHATTER_H
#define CHATTER_FIRMWARE_INPUTCHATTER_H

#include "InputLVGL.h"
#include <Input/InputListener.h>
#include <map>
#include <queue>

class InputChatter : public InputLVGL , public InputListener{
public:
	InputChatter();
	void read(lv_indev_drv_t* drv, lv_indev_data_t* data) override;

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

private:
	uint32_t lastKey = -1;
	bool pressed = false;
	static std::map<uint8_t, lv_key_t> keyMap;

};


#endif //CHATTER_FIRMWARE_INPUTCHATTER_H
