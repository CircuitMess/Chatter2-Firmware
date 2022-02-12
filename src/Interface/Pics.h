#ifndef CHATTER_FIRMWARE_PICS_H
#define CHATTER_FIRMWARE_PICS_H

#include <Arduino.h>
#include <lvgl.h>

#define NUM_PICS 8

struct Pic {
	const char* name;

	bool gif() const;
	lv_obj_t* create(lv_obj_t* parent) const;
};

extern const Pic Pics[NUM_PICS];


#endif //CHATTER_FIRMWARE_PICS_H
