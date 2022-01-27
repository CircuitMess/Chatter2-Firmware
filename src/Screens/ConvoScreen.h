#ifndef CHATTER_FIRMWARE_CONVOSCREEN_H
#define CHATTER_FIRMWARE_CONVOSCREEN_H

#include <Arduino.h>
#include "../LVScreen.h"
#include "../TextEntry.h"
#include "../Types.hpp"
#include "../Model/Profile.hpp"
#include "../Model/Convo.hpp"
#include "../Elements/ConvoBox.h"
#include "../WithListeners.h"

class ConvoScreen : public LVScreen, private InputListener {
public:
	ConvoScreen(UID_t uid);
	void onStart() override;
	void onStop() override;

private:
	void buttonPressed(uint i) override;
	Profile profile;
	const UID_t convo = 0;

	void send();

	ConvoBox* convoBox;
	TextEntry* entry;
};


#endif //CHATTER_FIRMWARE_CONVOSCREEN_H
