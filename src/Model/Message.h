#ifndef CHATTER_FIRMWARE_MESSAGE_H
#define CHATTER_FIRMWARE_MESSAGE_H

#include <Arduino.h>
#include "Entity.hpp"
#include <string>

class Message : Entity {
public:
	enum Type : uint8_t {
		TEXT, PIC, NONE
	};

	Message();
	Message(const std::string& text);
	Message(uint16_t picIndex);
	Message(const Message& other);
	Message& operator=(const Message &other);
	virtual ~Message();

	UID_t uid = 0;
	UID_t convo = 0;
	bool outgoing = false;
	bool received = false;

	Type getType() const;
	std::string getText() const;
	uint16_t getPic() const;
	void setText(const std::string& text);
	void setPic(uint16_t index);

private:
	Type type = NONE;
	void* content = nullptr;

	void clear();
};


#endif //CHATTER_FIRMWARE_MESSAGE_H
