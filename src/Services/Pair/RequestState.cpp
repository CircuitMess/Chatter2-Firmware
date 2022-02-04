#include "RequestState.h"
#include "../LoRaPacket.h"
#include "../LoRaService.h"
#include "PairService.h"

RequestState::RequestState(UID_t uid, PairService* pairService) : State(pairService){
	RequestState::uid = uid;
	//generate my part of key
	for(int i = 0; i < 8; ++i){
		((uint32_t*)Pair->myKeyPart)[i] = LoRa.rand();
	}
	broadcastRand = LoRa.rand(0, 1000001);
	broadcastTime = broadcastInterval;
}


void RequestState::loop(uint micros){

	broadcastTime += micros;
	if(broadcastTime >= (broadcastInterval + broadcastRand)){
		broadcastTime = 0;
		broadcastRand = LoRa.rand(0, 1000001);
		auto packet = new RequestPair();
		memcpy(packet->encKey, Pair->myKeyPart, 32);
		LoRa.send(uid, LoRaPacket::PAIR_REQ, packet);
		delete packet;
	}


	ReceivedPacket<RequestPair> packet = LoRa.getPairRequest();
	if(!packet.content || packet.sender != uid) return;

	auto req = reinterpret_cast<RequestPair*>(packet.content);
	memcpy(Pair->friendKeyPart, req->encKey, 32);
	delete req;

	for(int i = 0; i < sizeof(Pair->pairKey); ++i){
		Pair->pairKey[i] =Pair->myKeyPart[i] ^ Pair->friendKeyPart[i];
	}
	Pair->requestRecieved();
}
