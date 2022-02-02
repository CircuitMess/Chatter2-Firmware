#ifndef CHATTER_FIRMWARE_FRIENDSSCREEN_H
#define CHATTER_FIRMWARE_FRIENDSSCREEN_H

#include "../LVScreen.h"
#include "../AutoPop.h"

class FriendsScreen : public LVScreen {
public:
	FriendsScreen();

	void onStart() override;
	void onStop() override;

private:
	AutoPop apop;

};


#endif //CHATTER_FIRMWARE_FRIENDSSCREEN_H
