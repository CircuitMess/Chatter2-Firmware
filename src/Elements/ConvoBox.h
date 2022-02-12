#ifndef CHATTER_FIRMWARE_CONVOBOX_H
#define CHATTER_FIRMWARE_CONVOBOX_H

#include <Arduino.h>
#include "../Interface/LVObject.h"
#include "../Interface/ConvoView.h"
#include "ConvoMessage.h"
#include "../Interface/LVSelectable.h"
#include "../Interface/WithListeners.h"
#include "../Services/MessageService.h"
#include "../Services/ProfileService.h"

#define EV_CONVOBOX_MSG_SELECTED ((lv_event_code_t) (_LV_EVENT_LAST + 1))
#define EV_CONVOBOX_CANCEL ((lv_event_code_t) (_LV_EVENT_LAST + 2))

class ConvoBox : public virtual LVObject, public LVSelectable, private MsgReceivedListener, private MsgChangedListener, private ProfileListener  {
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
	void profileChanged(const Friend &fren) override;

	void fillMessages();
	void checkScroll();
	void createMessage(const Message& msg);
};


#endif //CHATTER_FIRMWARE_CONVOBOX_H
