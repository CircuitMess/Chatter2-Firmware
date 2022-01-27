#ifndef CHATTER_FIRMWARE_PROFILESERVICE_H
#define CHATTER_FIRMWARE_PROFILESERVICE_H

#include <Arduino.h>
#include "../Types.hpp"
#include <Loop/LoopListener.h>
#include "LoRaPacket.h"
#include "../Model/Profile.hpp"
#include "ProfileListener.hpp"
#include <vector>
#include "../WithListeners.h"

class ProfileService : public LoopListener, public WithListeners<ProfileListener> {
public:
	void begin();
	void loop(uint micros) override;

	const Profile &getMyProfile() const;
	void setMyProfile(const Profile &myProfile);
	static size_t generateHash(const Profile &profile);
private:
	void sendResponse(UID_t receiver);
	void receiveResponse(ReceivedPacket<ProfilePacket> &packet);
	void sendRequest(UID_t receiver);

	Profile myProfile;
	uint32_t hashCheckTime = 0;
	const uint32_t hashCheckDelay = 10000000; // recheck profiles every 10s

	void checkHashes();
};

extern ProfileService Profiles;
#endif //CHATTER_FIRMWARE_PROFILESERVICE_H
