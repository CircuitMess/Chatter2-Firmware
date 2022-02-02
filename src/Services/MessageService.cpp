#include <Loop/LoopManager.h>
#include "MessageService.h"
#include "LoRaService.h"
#include "../Storage/Storage.h"

MessageService Messages;

void MessageService::begin(){
	LoopManager::addListener(this);

	for(UID_t uid : Storage.Convos.all()){
		Convo convo = Storage.Convos.get(uid);
		if(convo.uid == 0 || convo.messages.empty()) continue;

		Message msg = Storage.Messages.get(convo.messages.back());
		if(msg.uid == 0) continue;

		lastMessages.insert(std::make_pair(uid, msg));
	}
}

Message MessageService::sendText(UID_t convo, const std::string& text){
	Message message;
	message.setText(text);
	return sendMessage(convo, message);
}

Message MessageService::sendPic(UID_t convo, uint16_t index){
	Message message;
	message.setPic(index);
	return sendMessage(convo, message);
}

Message MessageService::sendMessage(UID_t uid, Message& message){
	if(!Storage.Friends.exists(uid)) return { };

	Convo convo = Storage.Convos.get(uid);
	if(convo.uid == 0) return { };

	do {
		message.uid = LoRa.randUID();
	} while(Storage.Messages.exists(message.uid));

	message.outgoing = true;

	if(!sendPacket(uid, message)) return { };

	if(!Storage.Messages.add(message)) return { };

	convo.messages.push_back(message.uid);
	if(!Storage.Convos.update(convo)) return { };

	lastMessages[convo.uid] = message;

	return message;
}

Message MessageService::resend(UID_t convo, UID_t message){
	if(!Storage.Convos.exists(convo)) return { };

	Message msg = Storage.Messages.get(message);
	if(msg.uid == 0) return { };
	if(msg.received || !msg.outgoing) return { };

	sendPacket(convo, msg);
	return msg;
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

bool MessageService::deleteMessage(UID_t convoUID, UID_t msgUID){
	Convo convo = Storage.Convos.get(convoUID);
	if(convo.uid == 0) return false;

	auto pos = std::find(convo.messages.begin(), convo.messages.end(), msgUID);
	if(pos == convo.messages.end()) return false;

	convo.messages.erase(pos);
	if(!Storage.Convos.update(convo)) return false;

	if(!Storage.Messages.remove(msgUID)) return false;

	return true;
}

Message MessageService::getLastMessage(UID_t convo){
	auto pair = lastMessages.find(convo);
	if(pair == lastMessages.end()) return { };
	return pair->second;
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

	Message existing = Storage.Messages.get(message.uid);
	if(existing.uid != 0){
		if(message.convo != existing.convo || message.getType() != existing.getType()){
			printf("Got message with already existing uid!\n");
			return;
		}

		// Message is already received, resend ACK packet
		MessagePacket ack;
		ack.type = MessagePacket::ACK;
		ack.uid = message.uid;
		LoRa.send(packet.sender, LoRaPacket::Type::MSG, &ack);
		return;
	}

	if(!Storage.Messages.add(message)){
		printf("Error adding message\n");
		return;
	}

	Convo convo = Storage.Convos.get(packet.sender);
	if(convo.uid == 0){
		convo.uid = packet.sender;
		Storage.Convos.add(convo);
	}

	convo.messages.push_back(message.uid);
	if(!Storage.Convos.update(convo)){
		printf("Error updating convo\n");
		Storage.Messages.remove(message.uid);
		return;
	}

	lastMessages[convo.uid] = message;

	for(auto listener : WithListeners<MsgReceivedListener>::getListeners()){
		listener->msgReceived(message);
	}

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

	for(auto listener : WithListeners<MsgChangedListener>::getListeners()){
		listener->msgChanged(msg);
	}
}

void MessageService::addReceivedListener(MsgReceivedListener* listener){
	WithListeners<MsgReceivedListener>::addListener(listener);
}

void MessageService::addChangedListener(MsgChangedListener* listener){
	WithListeners<MsgChangedListener>::addListener(listener);
}

void MessageService::removeReceivedListener(MsgReceivedListener* listener){
	WithListeners<MsgReceivedListener>::removeListener(listener);
}

void MessageService::removeChangedListener(MsgChangedListener* listener){
	WithListeners<MsgChangedListener>::removeListener(listener);
}
