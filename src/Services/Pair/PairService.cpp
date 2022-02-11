#include <vector>
#include "PairService.h"
#include <Loop/LoopManager.h>
#include "../LoRaService.h"
#include "BroadcastState.h"
#include "RequestState.h"
#include "AcknowledgeState.h"
#include "../ProfileService.h"
#include "../../Storage/Storage.h"

using namespace Pairing;

void PairService::begin(){
	if(state) return;

	LoRa.clearPairPackets();
	LoopManager::addListener(this);
	state = new BroadcastState(this);
	broadcastTime = broadcastInterval;
	broadcastRand = LoRa.rand(0, 1000001);
}

PairService::~PairService(){
	LoopManager::removeListener(this);
	delete state;
	state = nullptr;
	if(friendStored && !done){
		//remove friend and revert encryption keys
		Storage.Friends.remove(pairUID);
		LoRa.copyEncKeys();
		friendStored = false;
	}
	LoRa.clearPairPackets();
}

void PairService::loop(uint micros){
	//don't broadcast adverts when listening for acks and reqs
	if(!friendStored){
		broadcastTime += micros;
		if(broadcastTime >= (broadcastInterval + broadcastRand)){
			broadcastTime = 0;
			broadcastRand = LoRa.rand(0, 1000001);
			sendAdvert();
		}
	}

	if(!state) return;
	state->loop(micros);

}

const std::vector<Profile> &PairService::getFoundProfiles() const{
	return foundProfiles;
}

const std::vector<UID_t> &PairService::getFoundUIDs() const{
	return foundUIDs;
}

void PairService::requestPair(uint32_t index){
	//check if pair already requested?
	if(friendStored || pairUID != 0 || index >= foundUIDs.size()) return;

	pairUID = foundUIDs[index];
	delete state;
	state = new RequestState(pairUID, this);

}

void PairService::sendAdvert(){
	auto packet = new AdvertisePair(Profiles.getMyProfile());
	LoRa.send(0, LoRaPacket::PAIR_BROADCAST, packet);
	delete packet;
}

bool PairService::cancelPair(){
	if(friendStored || done) return false; //in ack state, cannot cancel

	delete state;
	LoRa.clearPairPackets();
	pairUID = 0;
	state = new BroadcastState(this);
	foundProfiles.clear();
	foundUIDs.clear();
}

void PairService::requestRecieved(){
	delete state;

	//add new friend to storage
	Profile prof = foundProfiles[std::find(foundUIDs.begin(), foundUIDs.end(), pairUID) - foundUIDs.begin()];
	Friend fren;
	fren.profile = prof;
	fren.uid = pairUID;
	memcpy(fren.encKey, pairKey, 32);
	if(Storage.Friends.exists(pairUID)){
		Storage.Friends.update(fren);
	}else{
		Storage.Friends.add(fren);
	}
	friendStored = true;
	LoRa.copyEncKeys();

	//proceed to ack state
	state = new AcknowledgeState(pairUID, pairKey, this);

	if(responseCallback){
		responseCallback(responseCbData);
	}
}

void PairService::setDoneCallback(void (* doneCallback)(bool, void* pVoid), void* data){
	PairService::doneCallback = doneCallback;
	doneCbData = data;
}

void PairService::setResponseCallback(void (* responseCallback)(void*), void* data){
	PairService::responseCallback = responseCallback;
	responseCbData = data;
}

void PairService::pairDone(){
	delete state;
	state = nullptr;
	if(!Storage.Convos.exists(pairUID)){
		Convo convo;
		convo.uid = pairUID;
		Storage.Convos.add(convo);
	}
	LoRa.copyEncKeys();
	LoRa.clearPairPackets();

	done = true;
	if(doneCallback){
		doneCallback(true, doneCbData);
	}
}

void PairService::pairFailed(){
	LoRa.clearPairPackets();
	delete state;
	state = new BroadcastState(this);

	foundUIDs.clear();
	foundProfiles.clear();

	if(friendStored){
		//remove friend and revert encryption keys
		Storage.Friends.remove(pairUID);
		LoRa.copyEncKeys();
		friendStored = false;
	}
	pairUID = 0;

	if(doneCallback){
		doneCallback(false, doneCbData);
	}
}

void PairService::setUserFoundCallback(void ( * userFoundCallback)(const Profile &, void* pVoid), void* data){
	PairService::userFoundCallback = userFoundCallback;
	userFoundCbData = data;
}

void PairService::setUserChangedCallback(void (* userChangedCallback)(const Profile &, int index, void* pVoid), void* data){
	PairService::userChangedCallback = userChangedCallback;
	userChangedCbData = data;
}
