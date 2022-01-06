#ifndef CHATTER_FIRMWARE_AVATAR_H
#define CHATTER_FIRMWARE_AVATAR_H

#include "LVObject.h"

class Avatar{
public:
	Avatar(lv_obj_t* parent, uint8_t index, bool large = false);
	void changeImage(uint8_t index);
protected:
	lv_obj_t* obj;
	bool large = false;
};


#endif //CHATTER_FIRMWARE_AVATAR_H
