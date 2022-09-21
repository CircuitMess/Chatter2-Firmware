#include "ProfileService.h"
#include <Loop/LoopManager.h>
#include "LoRaService.h"
#include "../Storage/Storage.h"

ProfileService Profiles;
Profile temp = Profile{"Mauricije", 1, 100};
const uint8_t primeArray[17] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59};
const char* nameList[] = {"George", "Vicki", "Johnnie", "Michele", "Mandy", "Mark", "Bobbie", "Rene", "Michael", "Laura", "Ruby", "Erik", "Kim", "Hannah",
						  "Ellen", "Kevin", "Laurie", "Caleb", "Sarah", "Chester", "Dianna", "Lamar", "Bessie", "Phil", "Wanda"};

ProfileService::ProfileService(){
	reserve(8);
}

void ProfileService::loop(uint micros){

	hashCheckTime += micros;
	if(hashCheckTime >= hashCheckDelay){
		hashCheckTime = 0;
		checkHashes();
	}

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
		Profile defaultProfile = Profile{};
		strncpy(defaultProfile.nickname, nameList[LoRa.rand(sizeof(nameList) / sizeof(nameList[0]))], 15);
		defaultProfile.avatar = LoRa.rand(15);
		defaultProfile.hue = LoRa.rand(360);

		fren.profile = defaultProfile;
		fren.uid = ESP.getEfuseMac();
		if(!Storage.Friends.add(fren)){
			printf("Error applying default profile\n");
		}
		myProfile = defaultProfile;
	}else{
		myProfile = fren.profile;
	}
	myHash = generateHash(myProfile);
}

void ProfileService::sendResponse(UID_t receiver){
	auto packet = new ProfileResponse(myProfile);
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


	iterateListeners([&fren](ProfileListener* listener){
		listener->profileChanged(fren);
	});
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

	myHash = generateHash(myProfile);
}

size_t ProfileService::generateHash(const Profile &profile){
	size_t result = 0;
	for(int i = 0, j = 0; i < sizeof(profile); ++i, j = (j+1) % sizeof(primeArray)){
		result += ((char*)&profile)[i] * primeArray[j];
	}
	return result;
}

void ProfileService::checkHashes(){
	auto hashmap = LoRa.getHashmapCopy();
	for(auto const &pair: *hashmap){
		if(pair.first == ESP.getEfuseMac()) continue;

		if(generateHash(Storage.Friends.get(pair.first).profile) != pair.second){
			sendRequest(pair.first);
		}
	}

}

void ProfileService::sendRequest(UID_t receiver){
	auto packet = new ProfilePacket;
	packet->type = ProfilePacket::REQ;
	LoRa.send(receiver, LoRaPacket::PROF, packet);
	delete packet;
}

size_t ProfileService::getMyHash() const{
	return myHash;
}

