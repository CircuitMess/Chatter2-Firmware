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

	Friend fren = Storage.Friends.get(ESP.getEfuseMac());
	if(fren.uid == 0){
		Profile defaultProfile = Profile{"Name",  0, 0};
		fren.profile = defaultProfile;
		fren.uid = ESP.getEfuseMac();
		if(!Storage.Friends.update(fren)){
			printf("Error applying default profile\n");
		}
		myProfile = defaultProfile;
	}else{
		myProfile = fren.profile;
	}
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

const Profile &ProfileService::getMyProfile() const{
	return myProfile;
}

void ProfileService::setMyProfile(const Profile &myProfile){
	ProfileService::myProfile = myProfile;

	Friend fren = Storage.Friends.get(ESP.getEfuseMac());

	fren.profile = myProfile;

	if(!Storage.Friends.update(fren)){
		printf("Error updating my profile\n");
	}
}

