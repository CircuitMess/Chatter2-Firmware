#include "AcknowledgeState.h"
#include "../LoRaPacket.h"
#include "../LoRaService.h"
#include "PairService.h"

AcknowledgeState::AcknowledgeState(UID_t uid, uint8_t* key) : uid(uid){
	memcpy(this->key, key, 32);
}

void AcknowledgeState::loop(uint micros){
	broadcastTime += micros;
	if(broadcastTime >= broadcastInterval){
		broadcastTime = 0;
		auto packet = new RequestPair();
		memcpy(packet->encKey, Pair.myKeyPart, 32);
		LoRa.send(uid, LoRaPacket::PAIR_REQ, packet);
		delete packet;

		auto ack = new AckPair();
		memcpy(ack->encKey, Pair.pairKey, 32);
		LoRa.send(uid, LoRaPacket::PAIR_ACK, ack);
		delete ack;
	}

	ReceivedPacket<AckPair> packet = LoRa.getPairAck();
	if(!packet.content || packet.sender != uid) return;

	auto ack = reinterpret_cast<AckPair*>(packet.content);
	if(memcmp(ack->encKey, key, 32) == 0){
		ackCount++;
	}
	delete ack;
	if(ackCount >= 5){
		if(ackSuccessful > 1){
			Pair.pairDone();
		}else{
			Pair.cancelPair();
		}
	}

}
