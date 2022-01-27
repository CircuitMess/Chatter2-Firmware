#ifndef CHATTER_FIRMWARE_PROFILELISTENER_HPP
#define CHATTER_FIRMWARE_PROFILELISTENER_HPP
#include "../Model/Friend.hpp"

class ProfileListener{
public:
	virtual void profileChanged(const Friend& fren) = 0;
};
#endif //CHATTER_FIRMWARE_PROFILELISTENER_HPP
