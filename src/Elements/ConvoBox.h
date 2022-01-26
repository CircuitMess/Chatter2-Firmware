#ifndef CHATTER_FIRMWARE_CONVOBOX_H
#define CHATTER_FIRMWARE_CONVOBOX_H

#include <Arduino.h>
#include "../LVObject.h"
#include "../MessageView.h"
#include "../ConvoMessage.h"

class ConvoBox : public LVObject {
public:
	ConvoBox(lv_obj_t* parent, UID_t convo);

private:
	MessageView messageView;
	UID_t convo;

	std::vector<ConvoMessage*> msgElements;

	void fillMessages();

};


#endif //CHATTER_FIRMWARE_CONVOBOX_H
