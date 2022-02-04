#include "AcknowledgeState.h"
#include "../LoRaPacket.h"
#include "../LoRaService.h"
#include "PairService.h"

AcknowledgeState::AcknowledgeState(UID_t uid, uint8_t* key, PairService* pairService) : State(pairService){
	AcknowledgeState::uid = uid;
	memcpy(AcknowledgeState::key, key, 32);

	broadcastRand = LoRa.rand(0, 1000001);
	broadcastTime = 0;
}

void AcknowledgeState::loop(uint micros){

	ReceivedPacket<AckPair> packet = LoRa.getPairAck();
	if(packet.content && packet.sender == uid){
		auto ack = reinterpret_cast<AckPair*>(packet.content);
		if(memcmp(ack->encKey, key, 32) == 0){
			ackSuccessful++;
		}
		delete ack;
	}

	broadcastTime += micros;
	if(broadcastTime >= (broadcastInterval + broadcastRand)){
		if(whichPack){
			auto packet = new RequestPair();
			memcpy(packet->encKey, Pair->myKeyPart, 32);
			LoRa.send(uid, LoRaPacket::PAIR_REQ, packet);
			delete packet;
		}else{

			auto ack = new AckPair();
			memcpy(ack->encKey, Pair->pairKey, 32);
			LoRa.send(uid, LoRaPacket::PAIR_ACK, ack);
			delete ack;
			ackSent++;
		}
		broadcastTime = 0;
		broadcastRand = LoRa.rand(0, 1000001);
		whichPack = !whichPack;
	}


	if(ackSent >= 5){
		if(ackSuccessful > 0){
			Pair->pairDone();
		}else{
			Pair->pairFailed();
		}
	}

}
