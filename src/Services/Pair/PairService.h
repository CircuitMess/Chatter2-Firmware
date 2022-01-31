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
	void begin();
	void end();
	void loop(uint micros) override;

	const std::vector<Profile> & getFoundProfiles() const;

	void requestPair(UID_t uid);
	void cancelPair();
	void setDoneCallback(void (* doneCallback)(bool success));

private:
	State* state;

	uint8_t myKeyPart[32];
	uint8_t friendKeyPart[32];
	uint8_t pairKey[32];
	UID_t pairUID = 0;
	Profile pairProfile;
	std::vector<Profile> foundProfiles;
	std::vector<UID_t> foundUIDs;

	static void sendAdvert();
	uint32_t broadcastTime = 0;
	const uint32_t broadcastInterval = 1000000; //1s interval

	void requestRecieved();
	void pairDone();
	bool friendStored = false;

	void (* doneCallback)(bool success);
};

class PairListener {
	friend PairService;
private:
	virtual void friendPaired(const Friend &fren) = 0;
};

extern PairService Pair;
#endif //CHATTER_FIRMWARE_PAIRSERVICE_H
