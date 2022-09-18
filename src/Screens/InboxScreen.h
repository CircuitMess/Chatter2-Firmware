#ifndef CHATTER_FIRMWARE_INBOXSCREEN_H
#define CHATTER_FIRMWARE_INBOXSCREEN_H
#include "../Interface/LVScreen.h"
#include "vector"
#include "../AutoPop.h"
#include "../Types.hpp"

class UserWithMessage;

class InboxScreen : public LVScreen, private InputListener {
public:
	InboxScreen();

	void onStart() override;
	void onStop() override;
	void onStarting() override;

protected:
	AutoPop apop;

	struct LaunchParams {
		UID_t uid;
		InboxScreen* ctx;
	};

	std::vector<LaunchParams> params;

	std::vector<UserWithMessage*> userElements;

	void openConvo(UID_t uid);

private:
	void buttonPressed(uint i) override;
};


#endif //CHATTER_FIRMWARE_INBOXSCREEN_H
