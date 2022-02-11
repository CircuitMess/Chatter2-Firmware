#ifndef CHATTER_FIRMWARE_PROFILE_HPP
#define CHATTER_FIRMWARE_PROFILE_HPP

#include <Arduino.h>

struct Profile {
	char nickname[21];
	uint8_t avatar; //avatar image ID (0-14)
	uint16_t hue; //hue value (0-360)
};
#endif //CHATTER_FIRMWARE_PROFILE_HPP