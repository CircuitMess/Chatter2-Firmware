#include <SPIFFS.h>
#include "FSLVGL.h"
#include <FS/RamFile.h>

const char* FSLVGL::cached[] = {
		"/bg.bin",
		"/colorPicker.bin",
		"/Unread.bin",
		"/Menu/ArrowUp.bin",
		"/Menu/ArrowDown.bin",
		"/Menu/Small/Friends.bin",
		"/Menu/Small/Inbox.bin",
		"/Menu/Small/Profile.bin",
		"/Menu/Small/Settings.bin",
		"/Menu/Label/Friends.bin",
		"/Menu/Label/Inbox.bin",
		"/Menu/Label/Profile.bin",
		"/Menu/Label/Settings.bin"
};

std::unordered_map<std::string, fs::File*> FSLVGL::cache;
fs::File* FSLVGL::specialCache = nullptr;
bool FSLVGL::cacheLoaded = false;

FSLVGL::FSLVGL(fs::FS &filesystem, char letter) : filesys(filesystem){
	lv_fs_drv_init(&drv);                     /*Basic initialization*/


	drv.letter = letter;                         /*An uppercase letter to identify the drive */
	drv.ready_cb = ready_cb;               /*Callback to tell if the drive is ready to use */
	drv.open_cb = open_cb;                 /*Callback to open a file */
	drv.close_cb = close_cb;               /*Callback to close a file */
	drv.read_cb = read_cb;                 /*Callback to read a file */
	drv.write_cb = write_cb;               /*Callback to write a file */
	drv.seek_cb = seek_cb;                 /*Callback to seek in a file (Move cursor) */
	drv.tell_cb = tell_cb;                 /*Callback to tell the cursor position  */

	drv.dir_open_cb = dir_open_cb;         /*Callback to open directory to read its content */
	drv.dir_read_cb = dir_read_cb;         /*Callback to read a directory's content */
	drv.dir_close_cb = dir_close_cb;       /*Callback to close a directory */

	drv.user_data = this;             /*Any custom data if required*/

	lv_fs_drv_register(&drv);                 /*Finally register the drive*/
}

void FSLVGL::loadCache(){
	if(cacheLoaded) return;
	cacheLoaded = true;
	for(const char* path : cached){
		File file = SPIFFS.open(path);
		if(!file) continue;

		File* ram = new fs::File();
		*ram = RamFile::open(file);
		file.close();

		cache.insert(std::make_pair(path, ram));
	}
}

bool FSLVGL::ready_cb(struct _lv_fs_drv_t* drv){
	return true;
}

void* FSLVGL::open_cb(struct _lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode){
	auto entry = cache.find(path);
	if(entry != cache.end()){
		File* file = entry->second;
		file->seek(0);
		return file;
	}
	if(specialCache != nullptr){
		if(strcmp(specialCache->name(), path) == 0){
			specialCache->seek(0);
			return specialCache;
		}
	}

	const char* fsMode;
	switch(mode){
		case LV_FS_MODE_WR:
			fsMode = FILE_WRITE;
			break;
		default:
			fsMode = FILE_READ;
	}

	return (void*)new fs::File(static_cast<FSLVGL*>(drv->user_data)->getFS().open(path, fsMode));
}

fs::FS &FSLVGL::getFS(){
	return filesys;
}

lv_fs_res_t FSLVGL::close_cb(struct _lv_fs_drv_t* drv, void* file_p){
	if(cache.count(static_cast<File*>(file_p)->name())) return 0;
	if(specialCache == static_cast<File*>(file_p)) return 0;

	static_cast<fs::File*>(file_p)->close();
	delete static_cast<fs::File*>(file_p);
	return 0;
}

lv_fs_res_t FSLVGL::read_cb(struct _lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br){
	if(!static_cast<File*>(file_p)->operator bool()) return LV_FS_RES_NOT_EX;
	*br = static_cast<fs::File*>(file_p)->read((uint8_t*)buf, btr);
	return 0;
}

lv_fs_res_t FSLVGL::write_cb(struct _lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw){
	if(!static_cast<File*>(file_p)->operator bool()) return LV_FS_RES_NOT_EX;
	*bw = static_cast<fs::File*>(file_p)->write((uint8_t*)buf, btw);
	return 0;
}

lv_fs_res_t FSLVGL::seek_cb(struct _lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence){
	if(!static_cast<File*>(file_p)->operator bool()) return LV_FS_RES_NOT_EX;

	SeekMode mode;
	switch(whence){
		case LV_FS_SEEK_SET:
			mode = fs::SeekSet;
			break;
		case LV_FS_SEEK_CUR:
			mode = fs::SeekCur;
			break;
		case LV_FS_SEEK_END:
			mode = fs::SeekEnd;
			break;
	}
	if(!static_cast<fs::File*>(file_p)->seek(pos, mode)){
		return LV_FS_RES_INV_PARAM;
	}
	return 0;

}

lv_fs_res_t FSLVGL::tell_cb(struct _lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p){
	*pos_p = static_cast<fs::File*>(file_p)->position();
	if(*pos_p == (uint32_t)-1) return LV_FS_RES_UNKNOWN;
	return 0;
}

void* FSLVGL::dir_open_cb(struct _lv_fs_drv_t* drv, const char* path){
	return (void*)new fs::File(static_cast<FSLVGL*>(drv->user_data)->getFS().open(path));
}

lv_fs_res_t FSLVGL::dir_read_cb(struct _lv_fs_drv_t* drv, void* rddir_p, char* fn){
	if(!static_cast<fs::File*>(rddir_p)->isDirectory()) return LV_FS_RES_NOT_EX;
	fs::File temp = static_cast<fs::File*>(rddir_p)->openNextFile();
	if(!temp) return LV_FS_RES_NOT_EX;
	strncpy(fn, temp.name(), 256);
	return 0;
}

lv_fs_res_t FSLVGL::dir_close_cb(struct _lv_fs_drv_t* drv, void* rddir_p){
	static_cast<fs::File*>(rddir_p)->close();
	delete static_cast<fs::File*>(rddir_p);
	return 0;
}

void FSLVGL::loadSpecialCache(const char* path){
	unloadSpecialCache();
	File file = SPIFFS.open(path);
	if(!file) return;

	specialCache = new fs::File();
	*specialCache = RamFile::open(file);
	file.close();
}

void FSLVGL::unloadSpecialCache(){
	delete specialCache;
	specialCache = nullptr;
}




