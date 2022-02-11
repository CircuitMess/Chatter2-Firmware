#include "BroadcastState.h"
#include "../LoRaPacket.h"
#include "../LoRaService.h"
#include "PairService.h"

BroadcastState::BroadcastState(PairService* pService) : State(pService){
	buffTime = 0;
}

void BroadcastState::loop(uint micros){
	if(!buffCleared){
		buffTime += bufferClearTime;
		if(buffTime < bufferClearTime) return;
		buffCleared = true;
		LoRa.clearPairPackets();
	}
	ReceivedPacket<AdvertisePair> packet = LoRa.getPairBroadcast();

	if(!packet.content) return;

	auto advert = reinterpret_cast<AdvertisePair*>(packet.content);

	//check for newer Profile from same sender
	auto it = std::find(Pair->foundUIDs.begin(), Pair->foundUIDs.end(), packet.sender);
	if(it != Pair->foundUIDs.end()){
		if(memcmp(&Pair->foundProfiles[it - Pair->foundUIDs.begin()], &advert->profile, sizeof(Profile)) != 0){
			Pair->foundProfiles[it - Pair->foundUIDs.begin()] = advert->profile;
			if(Pair->userChangedCallback){
				Pair->userChangedCallback(Pair->foundProfiles[it - Pair->foundUIDs.begin()], it - Pair->foundUIDs.begin(), Pair->userChangedCbData);
			}
		}
	}else{
		Pair->foundUIDs.push_back(packet.sender);
		Pair->foundProfiles.push_back(advert->profile);
		if(Pair->userFoundCallback){
			Pair->userFoundCallback(Pair->foundProfiles.back(), Pair->userFoundCbData);
		}
	}

	delete advert;
}
