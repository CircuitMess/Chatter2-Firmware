#ifndef CIRCUITPET_FIRMWARE_RESOURCEMANAGER_H
#define CIRCUITPET_FIRMWARE_RESOURCEMANAGER_H

#include <Arduino.h>
#include <FS.h>
#include <map>
#include <string>

#define RES_HEART { "c/heart.raw", {}, true }
#define RES_GOBLET { "c/goblet.raw", {}, true }

#define FILE_HEART "/heart.raw"
#define FILE_GOBLET "/goblet.raw"

struct CompParams {
	uint8_t lookahead = 0;
	uint8_t expansion = 0;

	explicit operator bool() const{
		return lookahead && expansion;
	};
};

struct ResDescriptor {
	std::string path;
	CompParams compParams;
	bool inRam;
};

class ResourceManager {
public:
	ResourceManager(const char* root);
	virtual ~ResourceManager();
	void load(const std::vector<ResDescriptor>& descriptors);

	File getResource(std::string path);

private:
	std::map<std::string, File> resources;
	const char* root;
};


#endif //CIRCUITPET_FIRMWARE_RESOURCEMANAGER_H
