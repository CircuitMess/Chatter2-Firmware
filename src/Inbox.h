#ifndef CHATTER_FIRMWARE_INBOX_H
#define CHATTER_FIRMWARE_INBOX_H
#include "LVScreen.h"
#include "vector"

class UserWithMessage;

class Inbox : public LVScreen {
public:
	Inbox();
	void openConvo(uint32_t index);
	void newConvo();

protected:
	lv_obj_t* newConvoLayout;
	lv_obj_t* plusImg;
	lv_obj_t* newConvoLabel;
	std::vector<UserWithMessage*> userElements;
};


#endif //CHATTER_FIRMWARE_INBOX_H
