#include <SPIFFS.h>
#include <FS/RamFile.h>
#include "Repo.h"
#include "../Model/Message.h"
#include "../Model/Convo.hpp"
#include "../Model/Friend.hpp"

template class Repo<Message>;
template class Repo<Convo>;
template class Repo<Friend>;

template<typename T>
Repo<T>::Repo(const char* directory) : directory(directory){

}

template<typename T>
void Repo<T>::begin(bool cached){
	File dir = SPIFFS.open(directory);
	if(!dir){
		SPIFFS.mkdir(directory);
	}else if(!dir.isDirectory()){
		printf("Specified repo directory is a file: %s\n", directory);
	}
	dir.close();

	cache.clear();
	this->cached = cached;
	if(cached){
		for(UID_t uid : all(true)){
			get(uid, true);
		}
	}
}

template<typename T>
bool Repo<T>::add(const T& object){
	if(cached && cache.find(object.uid) != cache.end()) return false;

	const String path = getPath(object.uid);
	if(!cached && SPIFFS.exists(path)) return false;

	File file = SPIFFS.open(path, FILE_WRITE);
	if(!file) return false;

	if(!write(file, object)){
		file.close();
		SPIFFS.remove(path);
		return false;
	}

	if(cached){
		file.close();
		file = SPIFFS.open(path);
		cache.insert(std::make_pair(object.uid, RamFile::open(file, false)));
	}

	file.close();
	return true;
}

template<typename T>
bool Repo<T>::update(const T& object){
	if(!remove(object.uid)) return false;
	return add(object);
}

template<typename T>
bool Repo<T>::remove(UID_t uid){
	if(cached && cache.find(uid) == cache.end()) return false;

	const String path = getPath(uid);
	if(!cached && !SPIFFS.exists(path)) return false;

	cache.erase(uid);
	return SPIFFS.remove(path);
}

template<typename T>
T Repo<T>::get(UID_t uid, bool bypassCache){
	File file;

	if(cached && !bypassCache){
		auto pair = cache.find(uid);
		if(pair == cache.end()) return { };
		file = pair->second;
		file.seek(0);
	}else{
		const String path = getPath(uid);
		file = SPIFFS.open(path);
		if(!file) return { };
	}

	T object;
	if(!read(file, object)){
		file.close();
		return { };
	}

	object.uid = uid;

	if(cached && cache.find(uid) == cache.end()){
		cache.insert(std::make_pair(object.uid, RamFile::open(file, false)));
	}

	if(!cached || bypassCache){
		file.close();
	}

	return object;
}

template<typename T>
std::vector<UID_t> Repo<T>::all(bool bypassCache){
	std::vector<UID_t> list;

	if(cached && !bypassCache){
		for(const auto& pair : cache){
			list.push_back(pair.first);
		}

		return list;
	}

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

	std::reverse(list.begin(), list.end());

	return list;
}

template<typename T>
bool Repo<T>::exists(UID_t uid){
	if(cached) return cache.find(uid) != cache.end();
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

template<typename T>
void Repo<T>::clear(){
	for(auto el: all()){
		remove(el);
	}
	cache.clear();
}
