#ifndef CHATTER_FIRMWARE_EDITABLEAVATAR_H
#define CHATTER_FIRMWARE_EDITABLEAVATAR_H

#include "Avatar.h"

class EditableAvatar : public LVObject{
public:
	EditableAvatar(lv_obj_t* parent);
	void toggleState();
private:
	Avatar* avatar;
	lv_obj_t* arrowUp;
	lv_obj_t* arrowDown;
	uint8_t index = 0;
	const uint8_t numAvatars = 10;
	lv_timer_t* blinkTimer;

	bool arrowsState = false;
};


#endif //CHATTER_FIRMWARE_EDITABLEAVATAR_H
