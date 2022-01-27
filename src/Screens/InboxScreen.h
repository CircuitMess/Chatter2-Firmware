#ifndef CHATTER_FIRMWARE_INBOXSCREEN_H
#define CHATTER_FIRMWARE_INBOXSCREEN_H
#include "../LVScreen.h"
#include "vector"
#include "../AutoPop.h"
#include "../Types.hpp"
#include "../Services/ProfileListener.hpp"

class UserWithMessage;

class InboxScreen : public LVScreen, public ProfileListener {
public:
	InboxScreen();

	void onStart() override;
	void onStop() override;

	void profileChanged(const Friend &fren) override;

protected:
	AutoPop apop;

	struct LaunchParams {
		UID_t uid;
		InboxScreen* ctx;
	};

	std::vector<LaunchParams> params;

	lv_obj_t* newConvoLayout;
	lv_obj_t* plusImg;
	lv_obj_t* newConvoLabel;
	std::vector<UserWithMessage*> userElements;

	void openConvo(UID_t uid);
	void newConvo();
};


#endif //CHATTER_FIRMWARE_INBOXSCREEN_H
