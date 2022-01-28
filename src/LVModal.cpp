#include "LVModal.h"
#include "InputLVGL.h"

LVModal::LVModal(LVScreen* parent) : LVObject(parent->getLvObj()), parentScreen(parent){
	inputGroup = lv_group_create();

	lv_obj_add_flag(obj, LV_OBJ_FLAG_FLOATING);
	lv_obj_set_align(obj, LV_ALIGN_CENTER);
	lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);

	lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
	lv_obj_set_style_border_width(obj, 2, 0);
	lv_obj_set_style_border_opa(obj, LV_OPA_20, 0);
}

LVModal::~LVModal(){
	lv_group_del(inputGroup);
}

void LVModal::start(){
	lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), inputGroup);

	lv_group_focus_obj(lv_obj_get_child(obj, 0));

	active = true;
	lv_obj_invalidate(obj);
}

void LVModal::stop(){
	lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), parentScreen->getInputGroup());
	active = false;
}

bool LVModal::isActive() const{
	return active;
}
