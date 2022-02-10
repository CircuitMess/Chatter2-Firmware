#include "NotificationElement.h"

NotificationElement::NotificationElement(lv_obj_t* parent) : LVObject(parent){
	lv_obj_set_size(obj, 19, 18);

	img = lv_img_create(obj);
	lv_img_set_src(img, "S:/Unread.bin");
	lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);

	Messages.addUnreadListener(this);

	lv_anim_init(&anim);
	lv_anim_set_var(&anim, img);
	lv_anim_set_values(&anim, 0, 600);
	lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out);
	lv_anim_set_exec_cb(&anim, NotificationElement::notifFloat);
	lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_playback_time(&anim, 600);
	lv_anim_set_time(&anim, 600);
}

NotificationElement::~NotificationElement(){
	Messages.removeUnreadListener(this);
	stop();
}

void NotificationElement::notifFloat(void* var, int32_t value){
	lv_obj_t* obj = (lv_obj_t*) var;
	lv_obj_set_y(obj, round((float) value / 100.0f));
}

void NotificationElement::onUnread(bool unread){
	if(unread){
		lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
		lv_anim_start(&anim);
	}else{
		lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
		lv_anim_del(img,notifFloat);
	}

	lv_obj_invalidate(obj);
}

void NotificationElement::stop(){
	lv_anim_del(img,notifFloat);
}

void NotificationElement::start(){
	if(!Messages.hasUnread()){
		lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
	}else{
		lv_anim_start(&anim);
	}
}
