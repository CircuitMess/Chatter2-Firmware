#include "LVScreen.h"
#include "InputLVGL.h"

LVScreen::LVScreen() : LVObject(nullptr){
	lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		auto screen = static_cast<LVScreen*>(event->user_data);
		lv_indev_set_group(InputLVGL::getInstance()->getIndev(), screen->inputGroup);
		screen->onStart();
	}, LV_EVENT_SCREEN_LOADED, this);

	inputGroup = lv_group_create();
}

LVScreen::~LVScreen(){
	lv_group_del(inputGroup);
}

lv_group_t* LVScreen::getInputGroup(){
	return inputGroup;
}

void LVScreen::start(bool animate, lv_scr_load_anim_t anim){
	onStarting();

	if(animate){
		lv_scr_load_anim(obj, anim, 500, 0, false);
	}else{
		lv_scr_load(obj);
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
	if(parent == nullptr) return;

	stop();
	if(parent){
		parent->start(true, LV_SCR_LOAD_ANIM_OVER_TOP);
	}
	lv_obj_del_delayed(obj, 1000);
}


