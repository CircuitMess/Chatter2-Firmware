#ifndef CHATTER_FIRMWARE_PICMENU_H
#define CHATTER_FIRMWARE_PICMENU_H


#include "LVObject.h"

class PicMenu : public LVObject {
public:
	PicMenu(lv_obj_t* parent, void(*callback)(uint8_t picID, void* userData), void* userData);
	void exit(uint8_t picID);
	void enter(lv_group_t* oldGroup);

	void refreshArrows(uint8_t index);
private:
	void(*returnCallback)(uint8_t picID, void* userData);
	lv_group_t* oldGroup;
	lv_group_t* group;
	void* userData;

	lv_obj_t* arrowLeft;
	lv_obj_t* arrowRight;
	lv_anim_t anim;

	uint8_t index = 0;

};


#endif //CHATTER_FIRMWARE_PICMENU_H
