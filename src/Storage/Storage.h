#ifndef CHATTER_FIRMWARE_STORAGE_H
#define CHATTER_FIRMWARE_STORAGE_H

#include <Arduino.h>
#include "Repo.h"
#include "MessageRepo.h"
#include "ConvoRepo.h"
#include "../Model/Friend.hpp"

struct Repositories {
	MessageRepo Messages;
	ConvoRepo Convos;
	Repo<Friend> Friends;

	void begin();
};

extern Repositories Storage;


#endif //CHATTER_FIRMWARE_STORAGE_H
