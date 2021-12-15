#include "LVObject.h"

LVObject::LVObject(lv_obj_t* parent){
	obj = lv_obj_create(parent);
	lv_obj_add_event_cb(obj, [](lv_event_t * event){delete static_cast<LVObject*>(event->user_data);}, LV_EVENT_DELETE, this);
}

lv_obj_t* LVObject::getLvObj(){
	return obj;
}

