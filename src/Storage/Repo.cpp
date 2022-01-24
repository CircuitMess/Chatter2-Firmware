#include <SPIFFS.h>
#include "Repo.h"
#include "../Model/Message.h"
#include "../Model/Convo.hpp"
#include "../Model/Friend.hpp"

template class Repo<Message>;
template class Repo<Convo>;
template class Repo<Friend>;

template<typename T>
Repo<T>::Repo(const char* directory) : directory(directory){
	File dir = SPIFFS.open(directory);

	if(!dir){
		SPIFFS.mkdir(directory);
	}else if(!dir.isDirectory()){
		printf("Specified repo directory is a file: %s\n", directory);
		dir.close();
	}
}

template<typename T>
bool Repo<T>::add(const T& object){
	const String path = getPath(object.uid);
	if(SPIFFS.exists(path)) return false;

	File file = SPIFFS.open(path, FILE_WRITE);
	if(!file) return false;

	if(!write(file, object)){
		file.close();
		SPIFFS.remove(path);
		return false;
	}

	file.close();
	return true;
}

template<typename T>
bool Repo<T>::update(const T& object){
	const String path = getPath(object.uid);
	if(!SPIFFS.exists(path)) return false;

	if(!SPIFFS.remove(path)) return false;

	return add(object);
}

template<typename T>
bool Repo<T>::remove(UID_t uid){
	const String path = getPath(uid);
	if(!SPIFFS.exists(path)) return false;
	return SPIFFS.remove(path);
}

template<typename T>
T Repo<T>::get(UID_t uid){
	const String path = getPath(uid);
	if(!SPIFFS.exists(path)) return { };

	T object;

	File file = SPIFFS.open(path);
	if(!file) return { };

	if(!read(file, object)){
		file.close();
		return { };
	}

	file.close();

	object.uid = uid;
	return object;
}

template<typename T>
std::vector<UID_t> Repo<T>::all(){
	std::vector<UID_t> list;

	File root = SPIFFS.open(directory);
	if(!root || !root.isDirectory()){
		root.close();
		return { };
	}

	File entry;
	while((entry = root.openNextFile())){
		if(entry.isDirectory()){
			entry.close();
			continue;
		}

		String name = String(entry.name());
		name = name.substring(name.lastIndexOf('/') + 1);

		UID_t uid = strtoull(name.c_str(), nullptr, 16);
		list.push_back(uid);

		entry.close();
	}
	root.close();

	return list;
}

template<typename T>
bool Repo<T>::exists(UID_t uid){
	return SPIFFS.exists(getPath(uid));
}

template<typename T>
String Repo<T>::getPath(UID_t uid){
	char name[20] = {0};
	sprintf(name, "%016llx", uid);
	return directory + (directory[directory.length() - 1] == '/' ? "" : "/") + name;
}

template<typename T>
bool Repo<T>::write(File& file, const T& object){
	size_t written = file.write(reinterpret_cast<const uint8_t*>(&object), sizeof(T));
	return written == sizeof(T);
}

template<typename T>
bool Repo<T>::read(File& file, T& object){
	size_t read = file.read(reinterpret_cast<uint8_t*>(&object), sizeof(T));
	return read == sizeof(T);
}
