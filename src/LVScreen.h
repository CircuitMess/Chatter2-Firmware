#ifndef CHATTER_FIRMWARE_LVSCREEN_H
#define CHATTER_FIRMWARE_LVSCREEN_H

#include <lvgl.h>
#include "LVObject.h"

class LVScreen : public LVObject {
public:
	LVScreen();
	virtual ~LVScreen();

	virtual void onStart(){};
	virtual void onStop(){};

	void start();
	void stop();

	void push(LVScreen* other);
	void pop();

	lv_group_t* getInputGroup();
	void onLvScreenDelete();

protected:
	lv_group_t* inputGroup;

	LVScreen* parent = nullptr;
};


#endif //CHATTER_FIRMWARE_LVSCREEN_H
