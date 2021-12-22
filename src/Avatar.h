#ifndef CHATTER_FIRMWARE_AVATAR_H
#define CHATTER_FIRMWARE_AVATAR_H

#include "LVObject.h"

class Avatar : public LVObject {
public:
	Avatar(lv_obj_t* parent, uint8_t index);
	void changeImage(uint8_t index);
};


#endif //CHATTER_FIRMWARE_AVATAR_H
