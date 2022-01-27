#ifndef CHATTER_FIRMWARE_PROFILESERVICE_H
#define CHATTER_FIRMWARE_PROFILESERVICE_H

#include <Arduino.h>
#include "../Types.hpp"
#include <Loop/LoopListener.h>
#include "LoRaPacket.h"
#include "../Model/Profile.hpp"

class ProfileService : public LoopListener {
public:
	void begin();
	void loop(uint micros) override;

	const Profile &getMyProfile() const;
	void setMyProfile(const Profile &myProfile);
private:
	void sendResponse(UID_t receiver);
	void receiveResponse(ReceivedPacket<ProfilePacket> &packet);

	Profile myProfile;
};

extern ProfileService Profiles;
#endif //CHATTER_FIRMWARE_PROFILESERVICE_H
