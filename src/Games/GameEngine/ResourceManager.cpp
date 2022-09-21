#include "ResourceManager.h"
#include <SPIFFS.h>
#include <FS/CompressedFile.h>
#include <FS/RamFile.h>

ResourceManager::ResourceManager(const char* root) : root(root){}

ResourceManager::~ResourceManager(){
	for(auto pair : resources){
		pair.second.close();
	}

	resources.clear();
}

void ResourceManager::load(const std::vector<ResDescriptor>& descriptors){
	uint8_t copyBuffer[1024];
	resources.reserve(descriptors.size());

	for(auto descriptor : descriptors){
		std::string path;

		if(descriptor.path[0] == 'c'){
			descriptor.path = descriptor.path.substr(1);
			path = "/Games/Common" + descriptor.path;
		}else{
			path = root + descriptor.path;
		}

		auto cPath = path.c_str();

		File original = SPIFFS.open(cPath);
		if(!original){
			ESP_LOGE("ResMan", "Failed to load resource %s", cPath);
			continue;
		}

		if(descriptor.inRam){
			if(descriptor.compParams){

				//decode compressed file from SPIFFS to decoded RAMFile
				original = CompressedFile::open(original, descriptor.compParams.expansion, descriptor.compParams.lookahead);
				original.seek(0);
				File output = RamFile::create();

				while(size_t readBytes = original.read(copyBuffer, sizeof(copyBuffer))){
					output.write(copyBuffer, readBytes);
				}
				resources[descriptor.path] = output;
			}else{
				//copy file from SPIFFS to RAMFile
				resources[descriptor.path] = RamFile::open(original);
			}

		}else{
			//use file from SPIFFS, not from RAM
			resources[descriptor.path] = original;
		}
	}
}

File ResourceManager::getResource(std::string path){
	auto file = resources.find(path);
	if(file == resources.end()) return { };
	file->second.seek(0);
	return file->second;
}

