#ifndef CHATTER_FIRMWARE_PROFILE_HPP
#define CHATTER_FIRMWARE_PROFILE_HPP
#include <Arduino.h>

struct Profile{
	char nickname[20];
	uint8_t avatar; //avatar image ID
	uint8_t color; //hue value (0-180)
};
#endif //CHATTER_FIRMWARE_PROFILE_HPP