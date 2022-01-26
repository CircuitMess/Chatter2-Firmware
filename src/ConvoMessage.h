#ifndef CHATTER_FIRMWARE_CONVOMESSAGE_H
#define CHATTER_FIRMWARE_CONVOMESSAGE_H

#include <string>
#include "LVObject.h"

class ConvoMessage : public LVObject{
public:
	ConvoMessage(lv_obj_t* parent, const std::string& text, bool outgoing, uint8_t bgColor, bool delivered = false);
	void setDelivered(bool delivered);

protected:
	lv_style_t defaultStyle;
	lv_style_t focusedStyle;
	lv_obj_t* deliveredIndicator;
	bool delivered = false;
	bool outgoing = false;
};


#endif //CHATTER_FIRMWARE_CONVOMESSAGE_H
