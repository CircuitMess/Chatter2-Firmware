#ifndef CHATTER_FIRMWARE_LVSCREEN_H
#define CHATTER_FIRMWARE_LVSCREEN_H

#include <lvgl.h>

class LVScreen {
public:
	LVScreen();
	virtual ~LVScreen();

	virtual void onStart(){};
	virtual void onStop(){};

	void start();
	void stop();

	lv_group_t* getInputGroup();
	void onLvScreenDelete();


protected:
	lv_obj_t* screen;
	lv_group_t* inputGroup;

};


#endif //CHATTER_FIRMWARE_LVSCREEN_H
