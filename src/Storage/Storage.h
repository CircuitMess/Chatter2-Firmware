#ifndef CHATTER_FIRMWARE_STORAGE_H
#define CHATTER_FIRMWARE_STORAGE_H

#include <Arduino.h>
#include "Repo.h"
#include "MessageRepo.h"

struct Repositories {
	MessageRepo Messages;
};

extern Repositories Storage;


#endif //CHATTER_FIRMWARE_STORAGE_H
