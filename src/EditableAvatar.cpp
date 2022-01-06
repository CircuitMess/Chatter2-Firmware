#include "EditableAvatar.h"
#include "InputLVGL.h"
#include <Arduino.h>

#define AVATAR_NUM 15

void AnimCB(void* obj, int32_t v){
	lv_obj_set_style_translate_y((lv_obj_t*)obj, v, 0);
	lv_obj_invalidate(((lv_obj_t*)obj)->parent);
}

EditableAvatar::EditableAvatar(lv_obj_t* parent, uint8_t index, bool large) : LVObject(parent), index(index){

	this->index = min(this->index, (uint8_t)15);
	this->index = max(this->index, (uint8_t)1);

	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_style_pad_gap(obj, large ? 3 : 1, 0);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

	arrowUp = lv_img_create(obj);
	avatar = new Avatar(obj, this->index, large);
	arrowDown = lv_img_create(obj);
	lv_img_set_src(arrowUp, "S:/arrowUp.bin");
	lv_img_set_src(arrowDown, "S:/arrowDown.bin");
	lv_obj_set_style_opa(arrowUp, LV_OPA_0, 0);
	lv_obj_set_style_opa(arrowDown, LV_OPA_0, 0);

	lv_style_selector_t focusedSelector = LV_PART_MAIN | LV_STATE_FOCUSED;
	lv_obj_set_style_outline_opa(obj, LV_OPA_100, focusedSelector);

	lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_anim_init(&anim);
	lv_anim_set_var(&anim, arrowUp);
	lv_anim_set_playback_time(&anim, 100);
	lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_path_cb(&anim, lv_anim_path_step);
	lv_anim_set_exec_cb(&anim, AnimCB);
	lv_anim_set_values(&anim, -3, 0);
	lv_anim_set_values(&anim, 3, 0);
	lv_anim_set_var(&anim, arrowDown);


	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		static_cast<EditableAvatar*>(event->user_data)->toggleState();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		uint32_t c = lv_event_get_key(event);
		if(c == LV_KEY_LEFT){
			Serial.println("key up");
			static_cast<EditableAvatar*>(event->user_data)->scrollUp();
		}else if(c == LV_KEY_RIGHT){
			Serial.println("key down");
			static_cast<EditableAvatar*>(event->user_data)->scrollDown();
		}else if(c == LV_KEY_ESC){
			Serial.println("key esc");
			static_cast<EditableAvatar*>(event->user_data)->exit();
		}
	}, LV_EVENT_KEY, this);
}

void EditableAvatar::toggleState(){
	if(!arrowsState){
		lv_obj_set_style_opa(arrowUp, LV_OPA_100, 0);
		lv_obj_set_style_opa(arrowDown, LV_OPA_100, 0);

		if(index > 1){
			lv_anim_set_values(&anim, -3, 0);
			lv_anim_set_var(&anim, arrowUp);
			lv_anim_start(&anim);
		}

		if(index < 15){
			lv_anim_set_values(&anim, 3, 0);
			lv_anim_set_var(&anim, arrowDown);
			lv_anim_start(&anim);
		}

		lv_group_set_editing((lv_group_t*)lv_obj_get_group(obj), true);
//		lv_group_focus_freeze((lv_group_t*)lv_obj_get_group(obj), true);
//		lv_indev_set_group(InputLVGL::getInstance()->getIndev(), group);
//		lv_group_add_obj(group, obj);

	}else{
		exit();
	}
	lv_obj_invalidate(obj);
	arrowsState = !arrowsState;
}

void EditableAvatar::scrollUp(){
	index = max(--index, (uint8_t)1);
	Serial.println(index);
	avatar->changeImage(index);

	if(index < 15 && lv_anim_get(arrowDown, AnimCB) == nullptr){
		lv_anim_set_values(&anim, -3, 0);
		lv_anim_set_var(&anim, arrowUp);
		lv_anim_start(&anim);

		lv_anim_set_values(&anim, 3, 0);
		lv_anim_set_var(&anim, arrowDown);
		lv_anim_start(&anim);
	}else if(index == 1 && lv_anim_get(arrowUp, AnimCB) != nullptr){
		lv_anim_del(arrowUp, AnimCB);
	}
}

void EditableAvatar::scrollDown(){
	index = min(++index, (uint8_t)AVATAR_NUM);
	Serial.println(index);
	avatar->changeImage(index);

	if(index > 1 && lv_anim_get(arrowUp, AnimCB) == nullptr){

		lv_anim_set_values(&anim, 3, 0);
		lv_anim_set_var(&anim, arrowDown);
		lv_anim_start(&anim);

		lv_anim_set_values(&anim, -3, 0);
		lv_anim_set_var(&anim, arrowUp);
		lv_anim_start(&anim);
	}else if(index == 15 && lv_anim_get(arrowDown, AnimCB) != nullptr){
		lv_anim_del(arrowDown, AnimCB);
	}

}

void EditableAvatar::exit(){
	lv_obj_set_style_opa(arrowUp, LV_OPA_0, 0);
	lv_obj_set_style_opa(arrowDown, LV_OPA_0, 0);

	lv_anim_del(arrowUp, AnimCB);
	lv_anim_del(arrowDown, AnimCB);

	lv_group_set_editing((lv_group_t*)lv_obj_get_group(obj), false);

	lv_obj_invalidate(obj);
}
