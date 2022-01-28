#include "MessageRepo.h"

bool MessageRepo::write(File& file, const Message& object){
	Message::Type type = object.getType();

	size_t size = sizeof(object.uid) + sizeof(object.convo) + sizeof(object.outgoing) + sizeof(object.received) + sizeof(type);
	size_t totalWritten = 0;
	totalWritten += file.write(reinterpret_cast<const uint8_t*>(&object.uid), sizeof(object.uid));
	totalWritten += file.write(reinterpret_cast<const uint8_t*>(&object.convo), sizeof(object.convo));
	totalWritten += file.write(reinterpret_cast<const uint8_t*>(&object.outgoing), sizeof(object.outgoing));
	totalWritten += file.write(reinterpret_cast<const uint8_t*>(&object.received), sizeof(object.received));
	totalWritten += file.write(reinterpret_cast<const uint8_t*>(&type), sizeof(type));

	if(type == Message::TEXT){
		std::string text = object.getText();
		size_t textSize = text.size();
		size += textSize + sizeof(textSize);
		totalWritten += file.write(reinterpret_cast<const uint8_t*>(&textSize), sizeof(textSize));
		totalWritten += file.write(reinterpret_cast<const uint8_t*>(text.c_str()), textSize);
	}else if(type == Message::PIC){
		auto index = object.getPic();
		size += sizeof(index);
		totalWritten += file.write(reinterpret_cast<const uint8_t*>(&index), sizeof(index));
	}

	return size == totalWritten;
}

bool MessageRepo::read(File& file, Message& object){
	Message::Type type;
	size_t size = sizeof(object.uid) + sizeof(object.convo) + sizeof(object.outgoing) + sizeof(object.received) + sizeof(type);
	size_t totalRead = 0;
	totalRead += file.read(reinterpret_cast<uint8_t*>(&object.uid), sizeof(object.uid));
	totalRead += file.read(reinterpret_cast<uint8_t*>(&object.convo), sizeof(object.convo));
	totalRead += file.read(reinterpret_cast<uint8_t*>(&object.outgoing), sizeof(object.outgoing));
	totalRead += file.read(reinterpret_cast<uint8_t*>(&object.received), sizeof(object.received));
	totalRead += file.read(reinterpret_cast<uint8_t*>(&type), sizeof(type));

	if(type == Message::TEXT){
		size_t textSize;
		totalRead += file.read(reinterpret_cast<uint8_t*>(&textSize), sizeof(textSize));
		size += textSize + sizeof(textSize);

		char* data = static_cast<char*>(malloc(textSize + 1));
		data[textSize] = 0;

		totalRead += file.read(reinterpret_cast<uint8_t*>(data), textSize);
		object.setText(data);

		free(data);
	}else if(type == Message::PIC){
		object.setPic(UINT8_MAX);
		auto index = object.getPic();

		size += sizeof(index);
		totalRead += file.read(reinterpret_cast<uint8_t*>(&index), sizeof(index));

		object.setPic(index);
	}

	return size == totalRead;
}