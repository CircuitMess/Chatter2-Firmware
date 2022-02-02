#ifndef CHATTER_FIRMWARE_CONVOBOX_H
#define CHATTER_FIRMWARE_CONVOBOX_H

#include <Arduino.h>
#include "../LVObject.h"
#include "../ConvoView.h"
#include "../ConvoMessage.h"
#include "../LVSelectable.h"
#include "../WithListeners.h"
#include "../Services/MessageService.h"

#define EV_CONVOBOX_MSG_SELECTED ((lv_event_code_t) (_LV_EVENT_LAST + 1))

class ConvoBox : public virtual LVObject, public LVSelectable, private MsgReceivedListener, private MsgChangedListener {
public:
	ConvoBox(lv_obj_t* parent, UID_t convo, uint16_t hue = 0);
	virtual ~ConvoBox();

	void addMessage(const Message& msg);
	void removeMessage(UID_t uid);

	void load();

private:
	ConvoView convoView;
	UID_t convo;
	uint16_t hue;

	void enter();
	void exit();

	std::vector<ConvoMessage*> msgElements;

	lv_anim_t selectedAnim;
	void startAnim();
	void stopAnim();

	void msgReceived(const Message& msg) override;
	void msgChanged(const Message& message) override;

	void fillMessages();
	void checkScroll();
	void createMessage(const Message& msg);
};


#endif //CHATTER_FIRMWARE_CONVOBOX_H
