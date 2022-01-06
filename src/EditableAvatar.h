#ifndef CHATTER_FIRMWARE_EDITABLEAVATAR_H
#define CHATTER_FIRMWARE_EDITABLEAVATAR_H

#include "Avatar.h"

class EditableAvatar : public LVObject{
public:
	EditableAvatar(lv_obj_t* parent, uint8_t index = 1, bool large = false);
	void toggleState();
	void scrollUp();
	void scrollDown();
private:
	Avatar* avatar;
	lv_obj_t* arrowUp;
	lv_obj_t* arrowDown;
	uint8_t index = 1;
	bool arrowsState = false;
	lv_anim_t anim;

	void exit();
};


#endif //CHATTER_FIRMWARE_EDITABLEAVATAR_H
