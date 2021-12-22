#include "EditableAvatar.h"
#include <Arduino.h>

extern lv_anim_t anim;

void AnimCB(void* obj, int32_t v){
	lv_obj_set_style_translate_y((lv_obj_t*)obj, v, 0);
}
EditableAvatar::EditableAvatar(lv_obj_t* parent) : LVObject(parent){

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;

	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_style_pad_gap(obj, 1, sel);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	arrowUp = lv_img_create(obj);

	avatar = new Avatar(obj, index);

	arrowDown = lv_img_create(obj);
//	lv_img_set_src(arrowUp, "S:/arrowUp.bin");
//	lv_img_set_src(arrowDown, "S:/arrowDown.bin");

	lv_img_set_src(arrowUp, LV_SYMBOL_UP);
	lv_img_set_src(arrowDown, LV_SYMBOL_DOWN);
//	lv_obj_set_height(arrowUp, 12);
//	lv_obj_set_height(arrowDown, 12);
//	lv_img_set_offset_y(arrowUp, 5);
	lv_obj_set_style_border_width(obj, 1, sel);
	lv_obj_set_style_border_opa(obj, LV_OPA_100, sel);
	lv_obj_set_style_border_color(obj, lv_color_black(), sel);
	lv_obj_set_style_pad_bottom(arrowUp, -2, 0);
/*
	lv_obj_set_style_border_width(arrowUp, 1, sel);
	lv_obj_set_style_border_opa(arrowUp, LV_OPA_100, sel);
	lv_obj_set_style_border_color(arrowUp, lv_color_black(), sel);
	lv_obj_set_style_border_width(arrowDown, 1, sel);
	lv_obj_set_style_border_opa(arrowDown, LV_OPA_100, sel);
	lv_obj_set_style_border_color(arrowDown, lv_color_black(), sel);
*/


//	Serial.printf("arrowUp Y: %d\n", lv_obj_get_y(arrowUp));
//	lv_obj_set_y(arrowUp, 10);
//	lv_obj_set_height(arrowDown, 10);
//	lv_img_set_offset_x(arrowUp, 10);
	lv_obj_set_style_min_height(obj, 50, 0);

	lv_obj_add_flag(arrowUp, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(arrowDown, LV_OBJ_FLAG_HIDDEN);

	lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);

	lv_anim_init(&anim);
	lv_anim_set_var(&anim, arrowUp);
	lv_anim_set_playback_time(&anim, 100);
	lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_path_cb(&anim, lv_anim_path_step);
	lv_anim_set_exec_cb(&anim, AnimCB);
	lv_anim_set_values(&anim, 0, -3);

	lv_anim_set_values(&anim, 0, 3);
	lv_anim_set_var(&anim, arrowDown);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		lv_event_code_t code = lv_event_get_code(event);
		static_cast<EditableAvatar*>(event->user_data)->toggleState();
	}, LV_EVENT_VALUE_CHANGED, this);

}

void EditableAvatar::toggleState(){
	if(!arrowsState){
		lv_obj_clear_flag(arrowUp, LV_OBJ_FLAG_HIDDEN);
//		lv_obj_clear_flag(arrowDown, LV_OBJ_FLAG_HIDDEN);

/*		lv_anim_set_values(&anim, 0, -3);
		lv_anim_set_var(&anim, arrowUp);
		lv_anim_start(&anim);

		lv_anim_set_values(&anim, 0, 3);
		lv_anim_set_var(&anim, arrowDown);
		lv_anim_start(&anim);*/
	}else{
		lv_obj_add_flag(arrowUp, LV_OBJ_FLAG_HIDDEN);
//		lv_obj_add_flag(arrowDown, LV_OBJ_FLAG_HIDDEN);

/*		lv_anim_del(arrowUp, AnimCB);
		lv_anim_del(arrowDown, AnimCB);*/
	}
	arrowsState = !arrowsState;
}
