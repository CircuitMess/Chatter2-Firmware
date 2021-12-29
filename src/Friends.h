#ifndef CHATTER_FIRMWARE_FRIENDS_H
#define CHATTER_FIRMWARE_FRIENDS_H
#include "LVScreen.h"
#include "User.h"
#include <vector>

class Friends : public LVScreen {
public:
	Friends();
	virtual ~Friends();
protected:
	lv_obj_t* addLayout;
	lv_obj_t* plusImg;
	lv_obj_t* addFriendLabel;
	std::vector<User*> userElements;
};


#endif //CHATTER_FIRMWARE_FRIENDS_H
