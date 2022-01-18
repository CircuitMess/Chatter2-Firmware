#ifndef CHATTER_FIRMWARE_CONVOBOX_H
#define CHATTER_FIRMWARE_CONVOBOX_H

#include "LVObject.h"
#include "MessageView.hpp"
#include "LVScreen.h"

struct ConversationStruct;

class ConvoBox : public LVObject {
public:
	ConvoBox(ConversationStruct* convo, LVScreen* parent);
	void loadAbove();
	void loadBelow();
protected:
	ConversationStruct* convo;
	MessageView messageView;
private:
	void fillMessages();
	lv_group_t* inputGroup;
};


#endif //CHATTER_FIRMWARE_CONVOBOX_H
