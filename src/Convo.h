#ifndef CHATTER_FIRMWARE_CONVO_H
#define CHATTER_FIRMWARE_CONVO_H

#include <Arduino.h>
#include "LVScreen.h"
#include "TextEntry.h"
#include "Model/Profile.hpp"

class Convo : public LVScreen, private InputListener, public LoopListener {
public:
	Convo(const Profile& profile);
	void onStart() override;
	void onStop() override;

	void loop(uint micros) override;
private:
	void buttonPressed(uint i) override;
	Profile profile;

	lv_obj_t* messages;
	TextEntry* entry;
};


#endif //CHATTER_FIRMWARE_CONVO_H
