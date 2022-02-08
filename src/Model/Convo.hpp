#ifndef CHATTER_FIRMWARE_CONVO_HPP
#define CHATTER_FIRMWARE_CONVO_HPP

#include <Arduino.h>
#include "Entity.hpp"
#include <vector>

/// UID is same as friend UID
struct Convo : Entity {
	std::vector<UID_t> messages;
	bool unread = false;
};

#endif //CHATTER_FIRMWARE_CONVO_HPP
