#ifndef CHATTER_FIRMWARE_PAIRSERVICE_H
#define CHATTER_FIRMWARE_PAIRSERVICE_H

#include <Arduino.h>
#include "../../Types.hpp"
#include <Loop/LoopListener.h>
#include "../LoRaPacket.h"
#include "../../Model/Profile.hpp"
#include <vector>
#include "../../Interface/WithListeners.h"
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
	const std::vector<UID_t> & getFoundUIDs() const;
	void setUserFoundCallback(void ( * userFoundCallback)(const Profile &, void* pVoid), void* data);
	void setUserChangedCallback(void (* userChangedCallback)(const Profile &, int index, void* pVoid), void* data);
	void setDoneCallback(void (* doneCallback)(bool, void* pVoid), void* data);
	void setResponseCallback(void (* responseCallback)(void* pVoid), void* data);

	void requestPair(uint32_t index);
	bool cancelPair();

private:
	State* state = nullptr;

	uint8_t myKeyPart[32];
	uint8_t friendKeyPart[32];
	uint8_t pairKey[32];
	UID_t pairUID = 0;
	std::vector<Profile> foundProfiles;
	std::vector<UID_t> foundUIDs;

	static void sendAdvert();
	const uint32_t broadcastInterval = 2000000; //3s interval
	uint32_t broadcastTime = broadcastInterval;
	uint32_t broadcastRand = 0;

	void (* userFoundCallback)(const Profile &prof, void* data) = nullptr;
	void* userFoundCbData = nullptr;
	void (* userChangedCallback)(const Profile &prof, int index, void* data) = nullptr;
	void* userChangedCbData = nullptr;

	void requestRecieved();
	void pairDone();
	void pairFailed();
	bool friendStored = false;

	void (* doneCallback)(bool success, void* data) = nullptr;
	void* doneCbData = nullptr;

	void (* responseCallback)(void* data) = nullptr;
	void* responseCbData = nullptr;

	bool done = false;
};

class PairListener {
	friend PairService;
private:
	virtual void friendPaired(const Friend &fren) = 0;
};

#endif //CHATTER_FIRMWARE_PAIRSERVICE_H
