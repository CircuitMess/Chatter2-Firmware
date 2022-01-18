#ifndef CHATTER_FIRMWARE_FRIEND_HPP
#define CHATTER_FIRMWARE_FRIEND_HPP

#include "types.hpp"
#include "ProfileStruct.hpp"

struct Friend {
	ProfileStruct profile;
	bool inRange;
	UID_t uid;
	encKey_t encKey;
};
#endif //CHATTER_FIRMWARE_FRIEND_HPP
