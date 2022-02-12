#ifndef CHATTER_FIRMWARE_FRIENDSSCREEN_H
#define CHATTER_FIRMWARE_FRIENDSSCREEN_H

#include <vector>
#include "../Interface/LVScreen.h"
#include "../AutoPop.h"
#include "../Elements/User.h"

class FriendsScreen : public LVScreen{
public:
	FriendsScreen();

	void onStart() override;
	void onStop() override;
	void onStarting() override;

private:
	AutoPop apop;

	std::vector<User*> elements;

};


#endif //CHATTER_FIRMWARE_FRIENDSSCREEN_H
