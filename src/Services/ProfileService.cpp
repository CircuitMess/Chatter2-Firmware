#include "ProfileService.h"
#include <Loop/LoopManager.h>
#include "LoRaService.h"
#include "../Storage/Storage.h"

ProfileService Profiles;
Profile temp = Profile{"Mauricije", 1, 100};

void ProfileService::loop(uint micros){
	ReceivedPacket<ProfilePacket> packet = LoRa.getProfile();

	if(!packet.content || !Storage.Friends.exists(packet.sender)) return;

	if(packet.content->type == ProfilePacket::REQ){
		sendResponse(packet.sender);
	}else if(packet.content->type == ProfilePacket::RESP){
		receiveResponse(packet);
	}
}

void ProfileService::begin(){
	LoopManager::addListener(this);
}

void ProfileService::sendResponse(UID_t receiver){
	auto packet = new ProfileResponse(temp);
	LoRa.send(receiver, LoRaPacket::PROF, packet);
	delete packet;
}

void ProfileService::receiveResponse(ReceivedPacket<ProfilePacket> &packet){
	Profile profile = reinterpret_cast<ProfileResponse*>(packet.content)->profile;

	Friend fren = Storage.Friends.get(packet.sender);
	fren.profile = profile;

	if(!Storage.Friends.update(fren)){
		printf("Error updating friend\n");
	}

	// TODO: call Friend updated listeners
}

