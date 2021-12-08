#ifndef CHATTER_FIRMWARE_INPUTLVGL_H
#define CHATTER_FIRMWARE_INPUTLVGL_H

#include <lvgl.h>
class InputLVGL{
public:
	InputLVGL(lv_indev_type_t type = LV_INDEV_TYPE_KEYPAD);
	static InputLVGL* getInstance();

	virtual void read(lv_indev_drv_t* drv, lv_indev_data_t* data) = 0;
	lv_indev_t* getIndev();

private:
	static InputLVGL* instance;
	lv_indev_t* inputDevice;
};


#endif //CHATTER_FIRMWARE_INPUTLVGL_H
