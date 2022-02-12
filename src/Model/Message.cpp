#include "Message.h"

Message::Message(){

}

Message::Message(const std::string& text){
	setText(text);
}

Message::Message(uint8_t picIndex) {
	setPic(picIndex);
}

Message::Message(const Message& other) : uid(other.uid), convo(other.convo), outgoing(other.outgoing), received(other.received){
	clear();

	if(other.type == TEXT){
		setText(other.getText());
	}else if(other.type == PIC){
		setPic(other.getPic());
	}else{
		type = NONE;
	}
}

Message& Message::operator=(const Message& other){
	if(&other == this) return *this;

	uid = other.uid;
	convo = other.convo;
	outgoing = other.outgoing;
	received = other.received;

	clear();

	if(other.getType() == TEXT){
		setText(other.getText());
	}else if(other.getType() == PIC){
		setPic(other.getPic());
	}else{
		type = NONE;
	}

	return *this;
}

Message::~Message(){
	clear();
}

Message::Type Message::getType() const{
	return type;
}

std::string Message::getText() const{
	if(type != TEXT) return "";
	return *static_cast<const std::string*>(content);
}

uint8_t Message::getPic() const{
	if(type != PIC) return UINT8_MAX;
	return *static_cast<const uint8_t*>(content);
}

void Message::setText(const std::string& text){
	clear();
	type = TEXT;
	content = new std::string(text);
}

void Message::setPic(uint8_t index){
	clear();
	type = PIC;
	content = new uint8_t(index);
}

void Message::clear(){
	if(type == TEXT){
		delete static_cast<std::string*>(content);
	}else if(type == PIC){
		delete static_cast<uint8_t*>(content);
	}

	type = NONE;
	content = nullptr;
}