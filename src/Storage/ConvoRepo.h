#ifndef CHATTER_FIRMWARE_CONVOREPO_H
#define CHATTER_FIRMWARE_CONVOREPO_H

#include <Arduino.h>
#include "Repo.h"
#include "../Model/Convo.hpp"

class ConvoRepo : public Repo<Convo> {
public:
	 using Repo::Repo;

protected:
	bool write(File& file, const Convo& object) override;
	bool read(File& file, Convo& object) override;
};


#endif //CHATTER_FIRMWARE_CONVOREPO_H
