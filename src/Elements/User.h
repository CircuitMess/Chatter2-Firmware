#ifndef CHATTER_FIRMWARE_USER_H
#define CHATTER_FIRMWARE_USER_H

#include <Arduino.h>
#include <lvgl.h>
#include "../Interface/LVObject.h"
#include "../Model/Profile.hpp"
#include "../Services/ProfileService.h"

class Avatar;

class User : public LVObject, private ProfileListener {
public:
	User(lv_obj_t* parent, const Friend &fren);
	virtual ~User();
	UID_t getUID();
protected:
	lv_style_t styleFocus;
	lv_style_t styleDef;
	lv_obj_t* name;

	UID_t frenUID;
	Avatar* avatar;

private:
	void profileChanged(const Friend &fren) override;
};


#endif //CHATTER_FIRMWARE_USER_H
