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

class ConvoScreen : public LVScreen, private InputListener {
public:
	ConvoScreen(UID_t uid);
	void onStart() override;
	void onStop() override;

private:
	void buttonPressed(uint i) override;
	Profile profile;
	const UID_t convo = 0;

	void textEntryConfirm();
	void textEntryCancel();
	void convoBoxEnter();
	void convoBoxExit();
	void messageSelected(const Message& msg);
	void menuMessageSelected();
	void menuMessageCancel();
	void menuConvoSelected();
	void menuConvoCancel();
	void picMenuSelected();
	void picMenuCancel();

	bool loaded = false;

	ConvoBox* convoBox;
	TextEntry* textEntry;
	PicMenu* picMenu;
	ContextMenu* menuMessage;
	ContextMenu* menuConvo;

	Message selectedMessage;
};


#endif //CHATTER_FIRMWARE_CONVOSCREEN_H
