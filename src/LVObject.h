#ifndef CHATTER_FIRMWARE_LVOBJECT_H
#define CHATTER_FIRMWARE_LVOBJECT_H

#include <lvgl.h>

class LVObject {
public:
	LVObject(lv_obj_t* parent);
	virtual ~LVObject();

	lv_obj_t* getLvObj();

protected:
	lv_obj_t* obj = nullptr;

private:
	bool deleting = false;
};


#endif //CHATTER_FIRMWARE_LVOBJECT_H
