#ifndef CHATTER_FIRMWARE_MESSAGEREPO_H
#define CHATTER_FIRMWARE_MESSAGEREPO_H

#include <Arduino.h>
#include <FS.h>
#include "Repo.h"
#include "../Model/Message.h"

class MessageRepo : public Repo<Message> {
public:
	using Repo::Repo;

protected:
	bool write(fs::File& file, const Message& object) override;
	bool read(fs::File& file, Message& object) override;

};

#endif //CHATTER_FIRMWARE_MESSAGEREPO_H
