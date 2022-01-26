#ifndef CHATTER_FIRMWARE_CONVOSCREEN_H
#define CHATTER_FIRMWARE_CONVOSCREEN_H

#include <Arduino.h>
#include "../LVScreen.h"
#include "../TextEntry.h"
#include "../Types.hpp"
#include "../Model/Profile.hpp"
#include "../Model/Convo.hpp"

class ConvoScreen : public LVScreen, private InputListener, public LoopListener {
public:
	ConvoScreen(UID_t uid);
	void onStart() override;
	void onStop() override;

	void loop(uint micros) override;
private:
	void buttonPressed(uint i) override;
	Convo convo;
	Profile profile;

	lv_obj_t* messages;
	TextEntry* entry;
};


#endif //CHATTER_FIRMWARE_CONVOSCREEN_H
