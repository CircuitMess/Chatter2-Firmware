#include "LVScreen.h"
#include "InputLVGL.h"

LVScreen::LVScreen(){
	screen = lv_obj_create(nullptr);
	lv_obj_set_layout(screen, LV_LAYOUT_FLEX);
	inputGroup = lv_group_create();
	lv_obj_add_event_cb(screen, [](lv_event_t* event){
		auto screen = static_cast<LVScreen*>(event->user_data);
		screen->onLvScreenDelete();
		delete screen;

	}, LV_EVENT_DELETE, this);
}

LVScreen::~LVScreen(){
	if(screen != nullptr){
		lv_obj_del(screen);
	}
	lv_group_del(inputGroup);
}

void LVScreen::start(){
	lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0, false);
	lv_obj_add_event_cb(screen, [](lv_event_t* event){
		auto screen = static_cast<LVScreen*>(event->user_data);
		lv_indev_set_group(InputLVGL::getInstance()->getIndev(), screen->inputGroup);
		screen->onStart();
	}, LV_EVENT_SCREEN_LOADED, this);

}

void LVScreen::stop(){
	onStop();
}

lv_group_t* LVScreen::getInputGroup(){
	return inputGroup;
}

void LVScreen::onLvScreenDelete(){
	screen = nullptr;

}


