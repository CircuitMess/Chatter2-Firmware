#ifndef CHATTER_FIRMWARE_AVATAR_H
#define CHATTER_FIRMWARE_AVATAR_H

#include "../Interface/LVObject.h"

class Avatar : public LVObject{
public:
	Avatar(lv_obj_t* parent, uint8_t index, bool large = false);
	void changeImage(uint8_t index);
protected:
	bool large = false;
	lv_obj_t* img;
};


#endif //CHATTER_FIRMWARE_AVATAR_H
