#ifndef CHATTER_FIRMWARE_NOTIFICATIONELEMENT_H
#define CHATTER_FIRMWARE_NOTIFICATIONELEMENT_H

#include <Arduino.h>
#include "../Interface/LVObject.h"
#include "../Services/MessageService.h"

class NotificationElement : public LVObject, private UnreadListener {
public:
	NotificationElement(lv_obj_t* parent);
	virtual ~NotificationElement();

	void start();
	void stop();

private:
	lv_obj_t* img;
	lv_anim_t anim;

	static void notifFloat(void* var, int32_t value);

	void onUnread(bool unread) override;
};


#endif //CHATTER_FIRMWARE_NOTIFICATIONELEMENT_H
