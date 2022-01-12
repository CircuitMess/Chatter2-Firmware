#include "InputLVGL.h"
#include <lvgl.h>
//#include <Chatter.h>

InputLVGL* InputLVGL::instance = nullptr;

InputLVGL::InputLVGL(lv_indev_type_t type){
	instance = this;

	static lv_indev_drv_t inputDriver;
	lv_indev_drv_init(&inputDriver);
	inputDriver.type = type;
	inputDriver.long_press_repeat_time = UINT16_MAX;
	inputDriver.long_press_time = UINT16_MAX;
	inputDriver.read_cb = [](lv_indev_drv_t* drv, lv_indev_data_t* data){InputLVGL::getInstance()->read(drv, data);};
	inputDevice = lv_indev_drv_register(&inputDriver);
}

InputLVGL* InputLVGL::getInstance(){
	return instance;
}

lv_indev_t* InputLVGL::getIndev(){
	return inputDevice;
}
