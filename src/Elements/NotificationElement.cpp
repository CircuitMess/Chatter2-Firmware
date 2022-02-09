#include "NotificationElement.h"

NotificationElement::NotificationElement(lv_obj_t* parent) : LVObject(parent){
	lv_obj_set_size(obj, 19, 12);
	img = lv_img_create(obj);

	lv_img_set_src(img, "S:/Unread.bin");

	if(!Messages.hasUnread()){
		lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
	}

	Messages.addUnreadListener(this);
}

NotificationElement::~NotificationElement(){
	Messages.removeUnreadListener(this);
}

void NotificationElement::onUnread(bool unread){
	if(unread){
		lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
	}else{
		lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
	}

	lv_obj_invalidate(obj);
}
