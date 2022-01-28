#ifndef CHATTER_FIRMWARE_PICS_H
#define CHATTER_FIRMWARE_PICS_H

#include <Arduino.h>

#define NUM_PICS 15

struct Pic {
	const char* name;

	bool gif() const;
};

extern const Pic Pics[NUM_PICS];


#endif //CHATTER_FIRMWARE_PICS_H
