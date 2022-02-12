#include <Pins.hpp>
#include "LVSelectable.h"
#include "../InputLVGL.h"
#include <Input/Input.h>

LVSelectable::LVSelectable(lv_obj_t* parent) : LVObject(parent){
	inputGroup = lv_group_create();

	lv_obj_add_event_cb(obj, [](lv_event_t* e){
		LVSelectable* sel = static_cast<LVSelectable*>(e->user_data);
		sel->select();
	}, LV_EVENT_CLICKED, this);
}

LVSelectable::~LVSelectable(){
	lv_group_del(inputGroup);
}

void LVSelectable::select(){
	parentGroup = InputLVGL::getInstance()->getIndev()->group;
	active = true;

	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), inputGroup);
}

void LVSelectable::deselect(){
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), parentGroup);

	parentGroup = nullptr;
	active = false;

	lv_event_send(obj, LV_EVENT_READY, nullptr);
}

bool LVSelectable::isActive() const{
	return active;
}
