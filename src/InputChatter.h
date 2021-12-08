#ifndef CHATTER_FIRMWARE_INPUTCHATTER_H
#define CHATTER_FIRMWARE_INPUTCHATTER_H

#include "InputLVGL.h"
#include <Input/InputListener.h>
#include <map>

class InputChatter : public InputLVGL , public InputListener{
public:
	void read(lv_indev_drv_t* drv, lv_indev_data_t* data) override;

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;
private:
	std::map<lv_key_t, bool> keyState;

};


#endif //CHATTER_FIRMWARE_INPUTCHATTER_H
