#ifndef CHATTER_FIRMWARE_BATTERY_H
#define CHATTER_FIRMWARE_BATTERY_H

#include <Arduino.h>
#include "../LVObject.h"

class Battery : public LVObject {
public:
	Battery(lv_obj_t* parent);

private:
	lv_obj_t* img;
	uint8_t level = 3;
};


#endif //CHATTER_FIRMWARE_BATTERY_H
