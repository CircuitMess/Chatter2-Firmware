#include <Loop/LoopManager.h>
#include "MessageService.h"
#include "LoRaService.h"
#include "../Storage/Storage.h"

MessageService Messages;

void MessageService::begin(){
	LoopManager::addListener(this);
}

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

	if(!sendPacket(convo.uid, message)) return false;

	if(!Storage.Messages.add(message)) return false;

	convo.messages.push_back(message.uid);
	if(!Storage.Convos.update(convo)) return false;

	return true;
}

bool MessageService::sendPacket(UID_t receiver, const Message& message){
	MessagePacket* packet;

	if(message.getType() == Message::TEXT){
		packet = new TextMessage(message.getText());
	}else if(message.getType() == Message::PIC){
		packet = new PicMessage(message.getPic());
	}else{
		return false;
	}

	packet->uid = message.uid;
	LoRa.send(receiver, LoRaPacket::MSG, packet);

	delete packet;
	return true;
}

void MessageService::loop(uint micros){
	ReceivedPacket<MessagePacket> packet = LoRa.getMessage();

	if(!packet.content || !Storage.Friends.exists(packet.sender)) return;

	if(packet.content->type == MessagePacket::ACK){
		receiveAck(packet);
	}else{
		receiveMessage(packet);
	}
}

void MessageService::receiveMessage(ReceivedPacket<MessagePacket>& packet){
	Message message;
	message.uid = packet.content->uid;
	message.outgoing = false;
	message.convo = packet.sender;

	if(packet.content->type == MessagePacket::TEXT){
		TextMessage* text = reinterpret_cast<TextMessage*>(packet.content);
		message.setText(text->text);
		delete text;
	}else if(packet.content->type == MessagePacket::PIC){
		PicMessage* pic = reinterpret_cast<PicMessage*>(packet.content);
		message.setPic(pic->index);
		delete pic;
	}else{
		return;
	}

	Convo convo = Storage.Convos.get(packet.sender);
	if(convo.uid == 0){
		convo.uid = packet.sender;
		Storage.Convos.add(convo);
	}

	convo.messages.push_back(message.uid);

	if(!Storage.Messages.add(message)){
		printf("Error adding message\n");
	}
	if(!Storage.Convos.update(convo)){
		printf("Error updating convo\n");
	}

	// TODO: call new message listeners

	MessagePacket ack;
	ack.type = MessagePacket::ACK;
	ack.uid = message.uid;

	LoRa.send(packet.sender, LoRaPacket::Type::MSG, &ack);
}

void MessageService::receiveAck(ReceivedPacket<MessagePacket>& packet){
	UID_t uid = packet.content->uid;
	delete packet.content;

	Message msg = Storage.Messages.get(uid);
	if(msg.uid == 0) return;

	msg.received = true;
	if(!Storage.Messages.update(msg)){
		printf("Message ACK update failed\n");
	}

	// TODO: call listeners
}
