#ifndef CHATTER_FIRMWARE_PAIRSERVICE_H
#define CHATTER_FIRMWARE_PAIRSERVICE_H

#include <Arduino.h>
#include "../../Types.hpp"
#include <Loop/LoopListener.h>
#include "../LoRaPacket.h"
#include "../../Model/Profile.hpp"
#include <vector>
#include "../../WithListeners.h"
#include "../../Model/Friend.hpp"
#include "State.h"

using namespace Pairing;

class PairListener;
class BroadcastState;
class RequestState;
class AcknowledgeState;

class PairService : public LoopListener, public WithListeners<PairListener> {
	friend BroadcastState;
	friend RequestState;
	friend AcknowledgeState;
public:
	virtual ~PairService();
	void begin();
	void loop(uint micros) override;

	const std::vector<Profile> & getFoundProfiles() const;
	void setUserFoundCallback(void (* userFoundCallback)(const Profile &));
	void setUserChangedCallback(void (* userChangedCallback)(const Profile &, int));

	void requestPair(uint32_t index);
	bool cancelPair();
	void setDoneCallback(void (* doneCallback)(bool success));

private:
	State* state = nullptr;

	uint8_t myKeyPart[32];
	uint8_t friendKeyPart[32];
	uint8_t pairKey[32];
	UID_t pairUID = 0;
	Profile pairProfile;
	std::vector<Profile> foundProfiles;
	std::vector<UID_t> foundUIDs;

	static void sendAdvert();
	uint32_t broadcastTime = 0;
	const uint32_t broadcastInterval = 3000000; //3s interval
	void (*userFoundCallback)(const Profile& prof) = nullptr;
	void (*userChangedCallback)(const Profile& prof, int) = nullptr;

	void requestRecieved();
	void pairDone();
	void pairFailed();
	bool friendStored = false;

	void (* doneCallback)(bool success) = nullptr;
};

class PairListener {
	friend PairService;
private:
	virtual void friendPaired(const Friend &fren) = 0;
};

#endif //CHATTER_FIRMWARE_PAIRSERVICE_H
