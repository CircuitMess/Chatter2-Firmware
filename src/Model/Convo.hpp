#ifndef CHATTER_FIRMWARE_CONVO_HPP
#define CHATTER_FIRMWARE_CONVO_HPP

#include <Arduino.h>
#include "Friend.hpp"

struct Convo {
	Friend pajdo;
	uint16_t messageCount;
};

#endif //CHATTER_FIRMWARE_CONVO_HPP
