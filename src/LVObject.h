#ifndef CHATTER_FIRMWARE_LVOBJECT_H
#define CHATTER_FIRMWARE_LVOBJECT_H

#include <lvgl.h>

class LVObject {
public:
	LVObject(LVObject* parent);

	lv_obj_t* getLvObj();
protected:
	lv_obj_t* obj = nullptr;
};


#endif //CHATTER_FIRMWARE_LVOBJECT_H
