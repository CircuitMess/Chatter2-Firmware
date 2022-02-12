#include "EditableAvatar.h"
#include "../InputLVGL.h"
#include <Arduino.h>

#define AVATAR_NUM 15 //0-14

void AnimCB(void* obj, int32_t v){
	lv_obj_set_style_translate_x((lv_obj_t*)obj, v, 0);
	lv_obj_invalidate(((lv_obj_t*)obj)->parent);
}

EditableAvatar::EditableAvatar(lv_obj_t* parent, uint8_t index, bool large) : LVObject(parent), index(index){

	this->index = min(this->index, (uint8_t)(AVATAR_NUM - 1));

	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
	lv_obj_set_style_pad_gap(obj, 1, 0);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_bg_opa(obj, LV_OPA_0, 0);

	arrowLeft = lv_img_create(obj);
	avatar = new Avatar(obj, this->index, large);
	arrowRight = lv_img_create(obj);
	lv_img_set_src(arrowLeft, LV_SYMBOL_LEFT);
	lv_img_set_src(arrowRight, LV_SYMBOL_RIGHT);
	lv_obj_set_style_opa(arrowLeft, LV_OPA_0, 0);
	lv_obj_set_style_opa(arrowRight, LV_OPA_0, 0);
	lv_obj_set_style_text_font(arrowLeft, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_font(arrowRight, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_color(arrowLeft, lv_color_white(), 0);
	lv_obj_set_style_text_color(arrowRight, lv_color_white(), 0);


/*	lv_obj_set_style_border_width(obj, 1, 0);
	lv_obj_set_style_border_color(obj, lv_color_white(), 0);
	lv_obj_set_style_border_opa(obj, LV_OPA_100, 0);*/
	//secure space for arrows moving
	lv_obj_update_layout(obj);
	lv_obj_set_width(obj, lv_obj_get_width(obj) + 6);


	lv_anim_init(&anim);
	lv_anim_set_var(&anim, arrowLeft);
	lv_anim_set_playback_time(&anim, 75);
	lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_path_cb(&anim, lv_anim_path_step);
	lv_anim_set_exec_cb(&anim, AnimCB);
	lv_anim_set_values(&anim, -2, 0);
	lv_anim_set_values(&anim, 2, 0);
	lv_anim_set_var(&anim, arrowRight);


	//toggle on click
	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		static_cast<EditableAvatar*>(event->user_data)->toggleState();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		uint32_t c = lv_event_get_key(event);
		auto editableAvatar = static_cast<EditableAvatar*>(event->user_data);
		switch(c){
			case LV_KEY_LEFT:
				editableAvatar->scrollUp();
				break;
			case LV_KEY_RIGHT:
				editableAvatar->scrollDown();
				break;
			case LV_KEY_ESC:
				editableAvatar->exit();
				break;
			default:
				break;
		}
	}, LV_EVENT_KEY, this);

	//focus forwarding to Avatar child
	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		lv_obj_add_state(lv_obj_get_child(lv_event_get_target(event), 1), LV_STATE_FOCUSED);
	}, LV_EVENT_FOCUSED, nullptr);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		lv_obj_clear_state(lv_obj_get_child(lv_event_get_target(event), 1), LV_STATE_FOCUSED);
	}, LV_EVENT_DEFOCUSED, nullptr);
}

void EditableAvatar::toggleState(){
	if(!arrowsState){
		lv_obj_set_style_opa(arrowLeft, LV_OPA_100, 0);
		lv_obj_set_style_opa(arrowRight, LV_OPA_100, 0);
		if(index > 0){
			lv_anim_set_values(&anim, -2, 0);
			lv_anim_set_var(&anim, arrowLeft);
			lv_anim_start(&anim);
		}
		if(index < (AVATAR_NUM - 1)){
			lv_anim_set_values(&anim, 2, 0);
			lv_anim_set_var(&anim, arrowRight);
			lv_anim_start(&anim);
		}
		lv_group_set_editing((lv_group_t*)lv_obj_get_group(obj), true);
		arrowsState = true;
	}else{
		exit();
	}
}

void EditableAvatar::scrollUp(){
	if(index == 0) return;
	avatar->changeImage(--index);

	lv_anim_del(arrowLeft, AnimCB);
	lv_anim_del(arrowRight, AnimCB);

	if(index != 0){
		lv_anim_set_values(&anim, -2, 0);
		lv_anim_set_var(&anim, arrowLeft);
		lv_anim_start(&anim);
	}

	lv_anim_set_values(&anim, 2, 0);
	lv_anim_set_var(&anim, arrowRight);
	lv_anim_start(&anim);
}

void EditableAvatar::scrollDown(){
	if(index == AVATAR_NUM - 1) return;
	avatar->changeImage(++index);

	lv_anim_del(arrowLeft, AnimCB);
	lv_anim_del(arrowRight, AnimCB);

	lv_anim_set_values(&anim, -2, 0);
	lv_anim_set_var(&anim, arrowLeft);
	lv_anim_start(&anim);

	if(index != AVATAR_NUM - 1){
		lv_anim_set_values(&anim, 2, 0);
		lv_anim_set_var(&anim, arrowRight);
		lv_anim_start(&anim);
	}
}

void EditableAvatar::exit(){
	lv_obj_set_style_opa(arrowLeft, LV_OPA_0, 0);
	lv_obj_set_style_opa(arrowRight, LV_OPA_0, 0);
	lv_anim_del(arrowLeft, AnimCB);
	lv_anim_del(arrowRight, AnimCB);

	lv_group_set_editing((lv_group_t*)lv_obj_get_group(obj), false);
	arrowsState = false;
}

uint8_t EditableAvatar::getIndex() const{
	return index;
}
