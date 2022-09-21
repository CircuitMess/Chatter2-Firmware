#ifndef CHATTER2_FIRMWARE_LOCKSCREEN_H
#define CHATTER2_FIRMWARE_LOCKSCREEN_H

#include <Arduino.h>
#include <memory>
#include <Input/InputListener.h>
#include "../Interface/LVScreen.h"
#include "../Elements/UnlockSlide.h"
#include "../Services/MessageService.h"
#include "../Elements/UserWithMessage.h"

class LockScreen : public LVScreen, private InputListener, private UnreadListener {
public:
	LockScreen();
	virtual ~LockScreen();

	void onStarting() override;
	void onStart() override;
	void onStop() override;

	static void activate(LVScreen* parent);

private:
	static LockScreen* instance;


	lv_obj_t* container;
	lv_obj_t* noUnreads = nullptr;

	UnlockSlide* slide;

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	void onUnread(bool unread) override;

	std::vector<UserWithMessage*> unreads;

	void loadUnread();
	void createNoUnreads();
	void clearUnreads();

};


#endif //CHATTER2_FIRMWARE_LOCKSCREEN_H
