#include <SPIFFS.h>
#include <FS/RamFile.h>
#include "UnitTests.h"
#include "Storage/Storage.h"

void fileTest(){
	uint8_t data[] = { 10, 11, 12, 13, 14, 15, 16, 18, 22, 32 };
	const char* filename = "/Tests/File";

	SPIFFS.remove(filename);
	File f = SPIFFS.open(filename, FILE_WRITE);
	f.write(data, sizeof(data));
	f.close();

	f = SPIFFS.open(filename);
	File ram = RamFile::open(f, false);
	f.close();

	ram.seek(1);
	for(int i = 1; i < sizeof(data); i++){
		uint8_t c;
		c = ram.read();
		if(c != data[i]){
			printf("1. Missmatch on byte %d. Got %d, expected %d\n", i, c, data[i]);
			return;
		}
	}
	printf("Test 1 OK\n");

	ram.write(data, sizeof(data));
	ram.seek(0);
	for(int i = 0, j = 0; i < 2 * sizeof(data); i++, j = (j+1) % sizeof(data)){
		uint8_t c;
		c = ram.read();
		if(c != data[j]){
			printf("2. Missmatch on byte %d. Got %d, expected %d\n", i, c, data[j]);
			return;
		}
	}
	printf("Test 2 OK\n");

	ram.seek(0, fs::SeekEnd);
	for(int i = 0; i < sizeof(data); i++){
		ram.write(data[i]);
	}

	uint8_t* buf = static_cast<uint8_t*>(malloc(3 * sizeof(data)));
	ram.seek(0);
	ram.read(buf, 3 * sizeof(data));
	for(int i = 0, j = 0; i < 3 * sizeof(data); i++, j = (j+1) % sizeof(data)){
		if(buf[i] != data[j]){
			printf("3. Missmatch on byte %d. Got %d, expected %d\n", i, buf[i], data[j]);
			return;
		}
	}
	printf("Test 3 OK\n");
}

void rmDir(fs::File& root){
	File file;
	while((file = root.openNextFile())){
		if(file.isDirectory()){
			rmDir(file);
		}else{
			String name = file.name();
			file.close();
			SPIFFS.remove(name);
		}
	}
	String name = root.name();
	root.close();
	SPIFFS.remove(name);
}

void clearStorage(){
	File root = SPIFFS.open("/Repo");
	rmDir(root);
}

void repoTest(){
#define check(msg, result) if(!(result)){ printf("Err: %s\n", (msg)); return; }

	clearStorage();
	Storage.Friends.begin(false);

	Friend fren;
	fren.uid = 152;
	check("Friend 1 add", Storage.Friends.add(fren));

	auto frens = Storage.Friends.all();
	check("Friends 1 get", frens.size() == 1);
	check("Friend 1 get", Storage.Friends.get(152).uid == 152);

	clearStorage();
	Storage.Friends.begin(true);
	check("Friends clear", Storage.Friends.all().empty());

	fren.uid = 152;
	fren.profile.hue = 11;
	check("Friend 2 add", Storage.Friends.add(fren));

	frens = Storage.Friends.all();
	check("Friends 2 get", frens.size() == 1);
	check("Friends 2 get uid", frens[0] == 152);
	fren = Storage.Friends.get(152);
	check("Friend 2 get", fren.uid == 152);
	check("Friend 2 get hue", fren.profile.hue == 11);

	Storage.Friends.begin(true);
	frens = Storage.Friends.all();
	check("Friends 3 get", frens.size() == 1);
	check("Friends 3 get uid", frens[0] == 152);
	fren = Storage.Friends.get(152);
	check("Friend 3 get", fren.uid == 152);
	check("Friend 3 get hue", fren.profile.hue == 11);

	fren.uid = 152;
	fren.profile.hue = 12;
	check("Friend 4 update", Storage.Friends.update(fren));
	frens = Storage.Friends.all();
	check("Friends 4 get", frens.size() == 1);
	check("Friends 4 get uid", frens[0] == 152);
	fren = Storage.Friends.get(152);
	check("Friend 4 get", fren.uid == 152);
	check("Friend 4 get hue", fren.profile.hue == 12);

	Storage.Friends.begin(true);
	frens = Storage.Friends.all();
	check("Friends 4 get", frens.size() == 1);
	check("Friends 4 get uid", frens[0] == 152);
	fren = Storage.Friends.get(152);
	check("Friend 4 get", fren.uid == 152);
	check("Friend 4 get hue", fren.profile.hue == 12);

	frens = Storage.Friends.all();
	check("Friends 5 get", frens.size() == 1);
	check("Friends 5 get uid", frens[0] == 152);
	fren = Storage.Friends.get(152);
	check("Friend 5 get", fren.uid == 152);
	check("Friend 5 get hue", fren.profile.hue == 12);
}