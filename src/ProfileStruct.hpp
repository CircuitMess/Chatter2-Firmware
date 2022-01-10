#ifndef CHATTER_FIRMWARE_PROFILESTRUCT_HPP
#define CHATTER_FIRMWARE_PROFILESTRUCT_HPP
#include <Arduino.h>

struct ProfileStruct{
	char nickname[20];
	uint8_t avatar; //avatar image ID
	uint8_t color; //hue value (0-180)
};
#endif //CHATTER_FIRMWARE_PROFILESTRUCT_HPP