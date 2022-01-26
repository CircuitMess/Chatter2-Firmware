#include "MessageView.h"

MessageView::MessageView(UID_t convo) : convoUID(convo){
		loadLatest();
}

void MessageView::load(size_t startIndex){
	messages.clear();
	this->startIndex = startIndex;

	Convo convo = Storage.Convos.get(convoUID);
	if(convo.uid == 0 || convo.messages.empty()) return;
	if(startIndex >= convo.messages.size()) return;

	size_t count = min((size_t) Count, convo.messages.size() - startIndex);

	for(size_t i = startIndex, j = 0; j < count; i++, j++){
		Message message = Storage.Messages.get(convo.messages[i]);
		if(message.uid == 0) continue;
		messages.push_back(message);
	}
}

void MessageView::loadLatest(){
	Convo convo = Storage.Convos.get(convoUID);
	if(convo.uid == 0) return;

	int32_t index = max(0, (int32_t) convo.messages.size() - Count);
	load(index);
}

const std::vector<Message>& MessageView::getMessages() const{
	return messages;
}

size_t MessageView::getStartIndex() const{
	return startIndex;
}
