#ifndef CHATTER_FIRMWARE_MAINMENU_H
#define CHATTER_FIRMWARE_MAINMENU_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include <vector>
#include "LVScreen.h"

class MainMenu : public LVScreen, public InputListener {
public:
	MainMenu();
	void onStart() override;
	void onStop() override;

private:
	struct Item {
		const char* label;
		const char* icon;
	} static const Items[];

	lv_obj_t* left;
	lv_obj_t* right;
	lv_obj_t* mid;

	std::vector<lv_obj_t*> bigs;
	std::vector<lv_obj_t*> smalls;
	std::vector<lv_anim_t> smallAnims;

	static void ease(void* obj, int32_t value);

	void selectNext();
	void selectPrev();
	void buttonPressed(uint i) override;

	void startAnim(uint8_t index, bool reverse = false);

	uint8_t selected = 0;
};


#endif //CHATTER_FIRMWARE_MAINMENU_H
