#include "LVObject.h"

LVObject::LVObject(LVObject* parent){
	obj = lv_obj_create(parent == nullptr ? nullptr : parent->getLvObj());
	lv_obj_add_event_cb(obj, [](lv_event_t * event){delete static_cast<LVObject*>(event->user_data);}, LV_EVENT_DELETE, this);
}

lv_obj_t* LVObject::getLvObj(){
	return obj;
}

