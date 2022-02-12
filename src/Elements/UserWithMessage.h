#ifndef CHATTER_FIRMWARE_USERWITHMESSAGE_H
#define CHATTER_FIRMWARE_USERWITHMESSAGE_H

#include "User.h"
#include <string>
#include "../Services/MessageService.h"

class UserWithMessage : public User, private MsgReceivedListener, private UnreadListener {
public:
	UserWithMessage(lv_obj_t* parent, const Friend &fren, const std::string& text = "");
	virtual ~UserWithMessage();

	void setText(const std::string& text);
	void updateText();

private:
	lv_obj_t* message;
	lv_obj_t* notif;

	void msgReceived(const Message &message) override;
	void onUnread(bool unread) override;

};


#endif //CHATTER_FIRMWARE_USERWITHMESSAGE_H
