#ifndef CHATTER_FIRMWARE_USERWITHMESSAGE_H
#define CHATTER_FIRMWARE_USERWITHMESSAGE_H

#include "User.h"
#include <string>
#include "Services/MessageService.h"

class UserWithMessage : public User, private MsgReceivedListener {
public:
	UserWithMessage(lv_obj_t* parent, const Friend &fren, const std::string& text);
	virtual ~UserWithMessage();
	void setText(const std::string& text);
private:
	void msgReceived(const Message &message) override;
	lv_obj_t* message;
};


#endif //CHATTER_FIRMWARE_USERWITHMESSAGE_H
