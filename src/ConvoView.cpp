#include "ConvoView.h"

ConvoView::ConvoView(UID_t convo) : convoUID(convo){

}

void ConvoView::load(size_t startIndex){
	messages.clear();
	this->startIndex = startIndex;

	Convo convo = Storage.Convos.get(convoUID);
	if(convo.uid == 0 || convo.messages.empty()) return;
	if(startIndex >= convo.messages.size()) return;

	if(startIndex + Count >= convo.messages.size()){
		startIndex = convo.messages.size() - Count;
	}
	this->startIndex = max(0, (int32_t) startIndex);

	latest = this->startIndex + Count >= convo.messages.size();

	totalMessageCount = convo.messages.size();
	size_t count = min((size_t) Count, convo.messages.size() - this->startIndex);

	for(size_t i = this->startIndex, j = 0; j < count; i++, j++){
		Message message = Storage.Messages.get(convo.messages[i]);
		if(message.uid == 0) continue;
		messages.push_back(message);
	}
}

void ConvoView::loadLatest(){
	Convo convo = Storage.Convos.get(convoUID);
	if(convo.uid == 0) return;

	int32_t index = max(0, (int32_t) convo.messages.size() - Count);
	load(index);
}

const std::vector<Message>& ConvoView::getMessages() const{
	return messages;
}

size_t ConvoView::getStartIndex() const{
	return startIndex;
}

size_t ConvoView::getTotalMessageCount() const{
	return totalMessageCount;
}

bool ConvoView::isLatest() const{
	return latest;
}
