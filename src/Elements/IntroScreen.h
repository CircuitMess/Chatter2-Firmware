#ifndef CHATTER_FIRMWARE_INTROSCREEN_H
#define CHATTER_FIRMWARE_INTROSCREEN_H

#include <Arduino.h>
#include "../Interface/LVScreen.h"

class IntroScreen : public LVScreen {
public:
	void onStop() override;
	void startAnim(void (* callback)() = nullptr);

private:
	lv_obj_t* gif;
	void (*callback)() = nullptr;

};


#endif //CHATTER_FIRMWARE_INTROSCREEN_H
