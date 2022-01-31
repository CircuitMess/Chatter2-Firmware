#include <vector>
#include "PairService.h"
#include <Loop/LoopManager.h>
#include "../LoRaService.h"
#include "BroadcastState.h"
#include "RequestState.h"
#include "AcknowledgeState.h"
#include "../ProfileService.h"
#include "../../Storage/Storage.h"

PairService Pair;
using namespace Pairing;

void PairService::begin(){
	LoopManager::addListener(this);

	state = new BroadcastState();

}

void PairService::end(){
	LoopManager::removeListener(this);
	delete state;
}

void PairService::loop(uint micros){
	broadcastTime += micros;
	if(broadcastTime >= broadcastInterval){
		broadcastTime = 0;
		Pair.sendAdvert();
	}

	if(!state) return;
	state->loop(micros);

}

const std::vector<Profile> &PairService::getFoundProfiles() const{
	return foundProfiles;
}

void PairService::requestPair(UID_t uid){
	if(friendStored || pairUID != 0) return;

	pairUID = uid;
	delete state;
	state = new RequestState(uid);

}

void PairService::sendAdvert(){
	auto packet = new AdvertisePair(Profiles.getMyProfile());
	LoRa.send(0, LoRaPacket::PAIR_BROADCAST, packet);
	delete packet;
}

void PairService::cancelPair(){
	end();

	if(friendStored){
		//remove friend and revert encryption keys
		Storage.Friends.remove(pairUID);
		LoRa.copyEncKeys();
	}

	if(doneCallback){
		doneCallback(false);
		doneCallback = nullptr;
	}
}

void PairService::requestRecieved(){
	delete state;

	Profile prof = foundProfiles[std::find(foundUIDs.begin(), foundUIDs.end(), pairUID) - foundUIDs.begin()];
	Friend fren;
	fren.profile = prof;
	fren.uid = pairUID;
	memcpy(fren.encKey, pairKey, 32);
	Storage.Friends.add(fren);
	friendStored = true;

	state = new AcknowledgeState(pairUID, pairKey);
}

void PairService::setDoneCallback(void (* doneCallback)(bool success)){
	PairService::doneCallback = doneCallback;
}

void PairService::pairDone(){
	end();
	if(doneCallback){
		doneCallback(true);
		doneCallback = nullptr;
	}
	LoRa.copyEncKeys();
}




