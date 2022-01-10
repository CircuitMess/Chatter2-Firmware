#ifndef CHATTER_FIRMWARE_CONVOMESSAGE_H
#define CHATTER_FIRMWARE_CONVOMESSAGE_H

#include "LVObject.h"

class ConvoMessage : public LVObject{
public:
	ConvoMessage(lv_obj_t* parent, bool outgoing, const char* content, uint8_t bgColor, bool delivered = false);
	void setDelivered(bool delivered);

protected:
	lv_style_t defaultStyle;
	lv_style_t focusedStyle;
	lv_obj_t* deliveredIndicator;
	bool delivered = false;
};


#endif //CHATTER_FIRMWARE_CONVOMESSAGE_H
