#ifndef CHATTER_FIRMWARE_CONVOBOX_H
#define CHATTER_FIRMWARE_CONVOBOX_H

#include <Arduino.h>
#include "../LVObject.h"
#include "../ConvoView.h"
#include "../ConvoMessage.h"

class ConvoBox : public LVObject {
public:
	ConvoBox(lv_obj_t* parent, UID_t convo);
	virtual ~ConvoBox();
	void focus();
	void defocus();

private:
	ConvoView messageView;
	UID_t convo;

	std::vector<ConvoMessage*> msgElements;

	lv_group_t* group;

	void fillMessages();
	void checkScroll();

};


#endif //CHATTER_FIRMWARE_CONVOBOX_H
