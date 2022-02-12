#ifndef CHATTER_FIRMWARE_LVSELECTABLE_H
#define CHATTER_FIRMWARE_LVSELECTABLE_H

#include <Arduino.h>
#include <lvgl.h>
#include <Input/InputListener.h>
#include "LVObject.h"

class LVSelectable : virtual public LVObject {
public:
	LVSelectable(lv_obj_t* parent);
	virtual ~LVSelectable();

	bool isActive() const;
	void select();
	void deselect();

protected:
	lv_group_t* inputGroup;

private:
	lv_group_t* parentGroup = nullptr;
	bool active = false;
};


#endif //CHATTER_FIRMWARE_LVSELECTABLE_H
