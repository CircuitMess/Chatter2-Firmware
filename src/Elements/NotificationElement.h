#ifndef CHATTER_FIRMWARE_NOTIFICATIONELEMENT_H
#define CHATTER_FIRMWARE_NOTIFICATIONELEMENT_H

#include <Arduino.h>
#include "../LVObject.h"
#include "../Services/MessageService.h"

class NotificationElement : public LVObject, private UnreadListener {
public:
	NotificationElement(lv_obj_t* parent);
	virtual ~NotificationElement();

private:
	lv_obj_t* img;

	void onUnread(bool unread) override;
};


#endif //CHATTER_FIRMWARE_NOTIFICATIONELEMENT_H
