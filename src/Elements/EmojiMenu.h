#ifndef CHATTER_FIRMWARE_EMOJIMENU_H
#define CHATTER_FIRMWARE_EMOJIMENU_H

#include "../Interface/LVObject.h"

class EmojiMenu : public LVObject {
public:
	EmojiMenu(lv_obj_t* parent, void(*callback)(uint8_t emojiID, void* userData), void* userData);
	void exit(uint8_t emojiID);
	void enter(lv_group_t* oldGroup);
private:
	void(*returnCallback)(uint8_t emojiID, void* userData);
	lv_group_t* group;
	lv_group_t* oldGroup;
	void* userData;
};


#endif //CHATTER_FIRMWARE_EMOJIMENU_H
