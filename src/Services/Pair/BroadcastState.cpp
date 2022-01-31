#include "BroadcastState.h"
#include "../LoRaPacket.h"
#include "../LoRaService.h"
#include "PairService.h"

void BroadcastState::loop(uint micros){
	ReceivedPacket<AdvertisePair> packet = LoRa.getPairBroadcast();

	if(!packet.content) return;

	auto advert = reinterpret_cast<AdvertisePair*>(packet.content);

	//check for newer Profile from same sender
	auto it = std::find(Pair.foundUIDs.begin(), Pair.foundUIDs.end(), packet.sender);
	if(it != Pair.foundUIDs.end()){
		Pair.foundProfiles[it - Pair.foundUIDs.begin()] = advert->profile;
	}else{
		Pair.foundUIDs.push_back(packet.sender);
		Pair.foundProfiles.push_back(advert->profile);
	}

	delete advert;
}
