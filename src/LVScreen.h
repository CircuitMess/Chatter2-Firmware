#ifndef CHATTER_FIRMWARE_LVSCREEN_H
#define CHATTER_FIRMWARE_LVSCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVObject.h"

class LVScreen : public LVObject {
public:
	LVScreen();
	virtual ~LVScreen();

	virtual void onStarting();
	virtual void onStart();
	virtual void onStop();

	void start(bool animate = false, lv_scr_load_anim_t animation = LV_SCR_LOAD_ANIM_MOVE_BOTTOM);
	void stop();

	void push(LVScreen* other);
	void pop();

	lv_group_t* getInputGroup();

	void setParent(LVScreen* parent);
	LVScreen* getParent() const;

	static LVScreen* getCurrent();

protected:
	lv_group_t* inputGroup;

	LVScreen* parent = nullptr;

private:
	static LVScreen* current;

	bool running = false;

};


#endif //CHATTER_FIRMWARE_LVSCREEN_H
