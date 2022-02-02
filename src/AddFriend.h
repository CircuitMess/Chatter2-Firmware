#ifndef CHATTER_FIRMWARE_ADDFRIEND_H
#define CHATTER_FIRMWARE_ADDFRIEND_H

#include "LVScreen.h"
#include "vector"
#include "ProfileStruct.hpp"

class User;

class AddFriend : public LVScreen {
public:
	AddFriend();
	void selectUser(uint32_t index);
	void userFound(ProfileStruct user);

	void createSingleUser();
	void createUserList();
protected:
	lv_obj_t* pairAnimation;
	lv_obj_t* singleUserLayout;
	lv_obj_t* userList;

	void addUserToList(ProfileStruct& user);
};


#endif //CHATTER_FIRMWARE_ADDFRIEND_H
