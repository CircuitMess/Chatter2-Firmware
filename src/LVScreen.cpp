#include "LVScreen.h"
#include "InputLVGL.h"

LVScreen::LVScreen() : LVObject(nullptr){
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	inputGroup = lv_group_create();
	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		auto screen = static_cast<LVScreen*>(event->user_data);
		screen->onLvScreenDelete();
		delete screen;

	}, LV_EVENT_DELETE, this);
}

LVScreen::~LVScreen(){
	if(obj != nullptr){
		lv_obj_del(obj);
	}
	lv_group_del(inputGroup);
}

void LVScreen::onLvScreenDelete(){
	obj = nullptr;
}

lv_group_t* LVScreen::getInputGroup(){
	return inputGroup;
}

void LVScreen::start(bool animate){
	if(animate){
		lv_scr_load_anim(obj, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0, false);
		lv_obj_add_event_cb(obj, [](lv_event_t* event){
			auto screen = static_cast<LVScreen*>(event->user_data);
			lv_indev_set_group(InputLVGL::getInstance()->getIndev(), screen->inputGroup);
			screen->onStart();
		}, LV_EVENT_SCREEN_LOADED, this);
	}else{
		lv_scr_load(obj);
		lv_indev_set_group(InputLVGL::getInstance()->getIndev(), inputGroup);
		onStart();
	}
}

void LVScreen::stop(){
	onStop();
}

void LVScreen::push(LVScreen* other){
	other->parent = this;
	stop();
	other->start(true);
}

void LVScreen::pop(){
	stop();
	if(parent){
		parent->start(true);
	}
	lv_obj_del(obj);
}


