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
#include "../Modals/ContextMenu.h"
#include "../PicMenu.h"
#include "../Services/ProfileListener.hpp"

class ConvoScreen : public LVScreen, private InputListener, public ProfileListener {
public:
	ConvoScreen(UID_t uid);
	void onStart() override;
	void onStop() override;

	void profileChanged(const Friend &fren) override;

private:
	void buttonPressed(uint i) override;
	Profile profile;
	const UID_t convo = 0;

	void send();

	ConvoBox* convoBox;
	TextEntry* entry;
	PicMenu* picMenu;
	ContextMenu* menuResend;
	ContextMenu* menuConvo;

	Message selectedMessage;
};


#endif //CHATTER_FIRMWARE_CONVOSCREEN_H
