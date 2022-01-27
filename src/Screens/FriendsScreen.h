#ifndef CHATTER_FIRMWARE_FRIENDSSCREEN_H
#define CHATTER_FIRMWARE_FRIENDSSCREEN_H

#include "../LVScreen.h"
#include "../AutoPop.h"
#include "../Services/ProfileListener.hpp"

class FriendsScreen : public LVScreen, public ProfileListener {
public:
	FriendsScreen();

	void onStart() override;
	void onStop() override;

	void profileChanged(const Friend &fren) override;
private:
	AutoPop apop;

};


#endif //CHATTER_FIRMWARE_FRIENDSSCREEN_H
