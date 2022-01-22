#include "MessageService.h"
#include "LoRaService.h"
#include "../Storage/Storage.h"

bool MessageService::sendText(Convo& convo, const std::string& text){
	Message message;
	message.setText(text);
	return sendMessage(convo, message);
}

bool MessageService::sendPic(Convo& convo, uint16_t index){
	Message message;
	message.setPic(index);
	return sendMessage(convo, message);
}

bool MessageService::sendMessage(Convo& convo, Message& message){
	if(!Storage.Friends.exists(convo.uid)) return false;

	do {
		message.uid = LoRa.randUID();
	} while(Storage.Messages.exists(message.uid));

	message.outgoing = true;

	LoRa.send(convo.uid, LoRaPacket::Type::MSG, &message, sizeof(message));

	if(!Storage.Messages.add(message)) return false;
	if(!Storage.Convos.update(convo)) return false;

	return true;
}

void MessageService::loop(uint micros){
	ReceivedPacket<MessagePacket> packet = LoRa.getMessage();

	if(packet.content && Storage.Friends.exists(packet.sender)){
		if(packet.content->type == MessagePacket::ACK){
			// TODO
		}else{
			Message message;
			message.uid = packet.content->uid;
			message.outgoing = false;
			message.convo = packet.sender;

			if(packet.content->type == MessagePacket::TEXT){
				TextMessage* text = dynamic_cast<TextMessage*>(packet.content);
				message.setText(text->text);
				delete text;
			}else if(packet.content->type == MessagePacket::PIC){
				PicMessage* pic = reinterpret_cast<PicMessage*>(packet.content);
				message.setPic(pic->index);
				delete pic;
			}

			Convo convo = Storage.Convos.get(packet.sender);
			if(convo.uid == 0){
				convo.uid = packet.sender;
				Storage.Convos.add(convo);
			}

			Storage.Messages.add(message);
			Storage.Convos.update(convo);

			// TODO: call new message listeners
		}

		MessagePacket ack;
		ack.type = MessagePacket::ACK;
		ack.uid = packet.content->uid;

		LoRa.send(packet.sender, LoRaPacket::Type::MSG, &ack, sizeof(ack));
	}
}
