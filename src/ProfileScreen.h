#ifndef CHATTER_FIRMWARE_PROFILESCREEN_H
#define CHATTER_FIRMWARE_PROFILESCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVScreen.h"
#include <Input/InputListener.h>
#include "Model/Profile.hpp"

class TextEntry;
class EditableAvatar;
class ColorBox;

class ProfileScreen : public LVScreen, private InputListener{
public:
	ProfileScreen(const Profile& profile, bool editable = false);
	void onStart() override;
	void onStop() override;
private:
	TextEntry* name;
	EditableAvatar* editableAvatar;
	ColorBox* cbox;

	lv_style_t textStyle;
	bool editable = false;
	const Profile& profile;

	void buildHeader();
	void buildBody();
	void buildFooter();

	void buttonPressed(uint i) override;
};

#endif //CHATTER_FIRMWARE_PROFILESCREEN_H