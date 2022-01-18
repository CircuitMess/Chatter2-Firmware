#ifndef CHATTER_FIRMWARE_CONVO_H
#define CHATTER_FIRMWARE_CONVO_H

#include <Arduino.h>
#include "LVScreen.h"
#include "TextEntry.h"
#include "ProfileStruct.hpp"

class Convo : public LVScreen, private InputListener, public LoopListener {
public:
	Convo(const ProfileStruct& profile);
	void onStart() override;
	void onStop() override;

	void loop(uint micros) override;
private:
	void buttonPressed(uint i) override;
	ProfileStruct profile;

	lv_obj_t* messages;
	TextEntry* entry;
};


#endif //CHATTER_FIRMWARE_CONVO_H
