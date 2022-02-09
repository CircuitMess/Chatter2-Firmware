#include "ConvoRepo.h"

bool ConvoRepo::write(File& file, const Convo& convo){
	size_t count = convo.messages.size();
	size_t size = sizeof(count) + sizeof(convo.unread) + count * sizeof(UID_t);
	size_t written = 0;

	written += file.write(reinterpret_cast<const uint8_t*>(&convo.unread), sizeof(convo.unread));

	written += file.write(reinterpret_cast<const uint8_t*>(&count), sizeof(count));

	for(UID_t message : convo.messages){
		written += file.write(reinterpret_cast<const uint8_t*>(&message), sizeof(message));
	}

	return written == size;
}

bool ConvoRepo::read(File& file, Convo& convo){
	size_t read = 0;
	size_t count;
	size_t size = sizeof(count) + sizeof(convo.unread);

	read += file.read(reinterpret_cast<uint8_t*>(&convo.unread), sizeof(convo.unread));

	read += file.read(reinterpret_cast<uint8_t*>(&count), sizeof(count));

	if(read != size) return false;

	size += count * sizeof(UID_t);

	for(int i = 0; i < count; i++){
		UID_t message;
		read += file.read(reinterpret_cast<uint8_t*>(&message), sizeof(message));
		convo.messages.push_back(message);
	}

	return read == size;
}
