#ifndef CHATTER_FIRMWARE_INTROSCREEN_H
#define CHATTER_FIRMWARE_INTROSCREEN_H

#include <Arduino.h>
#include "LVScreen.h"

class IntroScreen : public LVScreen {
public:
	IntroScreen();
	void onStop() override;

	void startAnim();

private:
	lv_obj_t* splash;
	lv_obj_t* gif;

};


#endif //CHATTER_FIRMWARE_INTROSCREEN_H
