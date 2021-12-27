#ifndef CHATTER_FIRMWARE_USERWITHMESSAGE_H
#define CHATTER_FIRMWARE_USERWITHMESSAGE_H

#include "User.h"

class UserWithMessage : public User {
public:
	UserWithMessage(lv_obj_t* parent, const Profile &profile, const char* messageText);
};


#endif //CHATTER_FIRMWARE_USERWITHMESSAGE_H
