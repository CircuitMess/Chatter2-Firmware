#ifndef CHATTER_FIRMWARE_PROFILESCREEN_H
#define CHATTER_FIRMWARE_PROFILESCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../LVScreen.h"
#include <Input/InputListener.h>
#include "../Model/Friend.hpp"
#include "../Services/ProfileService.h"

class TextEntry;
class EditableAvatar;
class Avatar;
class ColorBox;

class ProfileScreen : public LVScreen, private InputListener, private ProfileListener{
public:
	ProfileScreen(UID_t uid, bool editable = false);
	void onStart() override;
	void onStop() override;
private:
	TextEntry* name;
	EditableAvatar* editableAvatar;
	Avatar* avatar;
	ColorBox* cbox;

	lv_style_t textStyle;
	bool editable = false;
	Friend frend;
	Profile& profile;

	void buildHeader();
	void buildBody();
	void buildFooter();

	void buttonPressed(uint i) override;

	void profileChanged(const Friend &fren) override;
};

#endif //CHATTER_FIRMWARE_PROFILESCREEN_H