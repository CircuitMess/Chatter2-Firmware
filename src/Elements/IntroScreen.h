#ifndef CHATTER_FIRMWARE_INTROSCREEN_H
#define CHATTER_FIRMWARE_INTROSCREEN_H

#include <Arduino.h>
#include "../Interface/LVScreen.h"

class IntroScreen : public LVScreen {
public:
	IntroScreen(void (* callback)());

	void onStart() override;
	void onStop() override;

private:
	lv_obj_t* gif;
	void (*callback)() = nullptr;

};


#endif //CHATTER_FIRMWARE_INTROSCREEN_H
