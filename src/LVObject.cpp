#include "LVObject.h"

LVObject::LVObject(lv_obj_t* parent){
	obj = lv_obj_create(parent);

	lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		auto obj = static_cast<LVObject*>(event->user_data);
		bool deleting = obj->isDeleting();
		obj->obj = nullptr;
		if(!deleting){
			delete obj;
		}
	}, LV_EVENT_DELETE, this);
}

LVObject::~LVObject(){
	if(obj != nullptr){
		lv_obj_t* _obj = obj;
		obj = nullptr;
		lv_obj_del(_obj);
	}
}

bool LVObject::isDeleting(){
	return obj == nullptr;
}

lv_obj_t* LVObject::getLvObj(){
	return obj;
}

