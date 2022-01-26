#ifndef CHATTER_FIRMWARE_FRIEND_HPP
#define CHATTER_FIRMWARE_FRIEND_HPP

#include <Arduino.h>
#include "Profile.hpp"
#include "Entity.hpp"
#include "../Types.hpp"

struct Friend : Entity {
	Profile profile;
	uint8_t encKey[32];
};

#endif //CHATTER_FIRMWARE_FRIEND_HPP
