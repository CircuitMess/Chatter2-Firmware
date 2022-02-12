#include "PicMenu.h"
#include <Arduino.h>
#include "../InputLVGL.h"
#include "../Interface/Pics.h"
#include "ConvoMessage.h"

void arrowAnim(void* obj, int32_t v){
	lv_obj_set_style_translate_x((lv_obj_t*)obj, v, 0);
}

PicMenu::PicMenu(LVScreen* parent) : LVModal(parent){
	lv_obj_set_style_bg_color(obj, lv_color_make(57, 49, 75), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_border_width(obj, 1, 0);
	lv_obj_set_style_border_color(obj, lv_color_white(), 0);
	lv_obj_set_style_border_opa(obj, LV_OPA_100, 0);
	lv_obj_set_size(obj, 116, 66);
	lv_obj_set_style_pad_hor(obj, 7, 0);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);

	picLayout = lv_obj_create(obj);
	lv_obj_set_layout(picLayout, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(picLayout, LV_FLEX_FLOW_ROW);
	lv_obj_set_style_pad_gap(picLayout, 32, 0);
	lv_obj_set_flex_align(picLayout, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_align(picLayout, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(picLayout, 64, 64);
	lv_obj_set_scrollbar_mode(picLayout, LV_SCROLLBAR_MODE_OFF);


	lv_group_set_wrap(inputGroup, false);

	inputGroup->user_data = this;
	lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
		auto* menu = static_cast<PicMenu*>(group->user_data);
		menu->refreshArrows();
		size_t index = lv_obj_get_index(lv_group_get_focused(group));
		lv_obj_scroll_to_x(menu->picLayout, index * (64 + 32), LV_ANIM_ON);
		printf("move CB\n");
	});

	for(const Pic picture : Pics){
		lv_obj_t* pic = picture.create(picLayout);
		pics.push_back(pic);

		lv_obj_clear_flag(pic, LV_OBJ_FLAG_CLICK_FOCUSABLE);
		lv_obj_clear_flag(pic, LV_OBJ_FLAG_CHECKABLE);
		lv_obj_clear_flag(pic, LV_OBJ_FLAG_SCROLLABLE);

		lv_group_add_obj(inputGroup, pic);

		lv_obj_add_event_cb(pic, [](lv_event_t* e){
			auto* menu = static_cast<PicMenu*>(e->user_data);
			menu->stop();
			lv_event_send(menu->getLvObj(), LV_EVENT_CLICKED, nullptr);
		}, LV_EVENT_CLICKED, this);

		lv_obj_add_event_cb(pic, [](lv_event_t* e){
			auto* menu = static_cast<PicMenu*>(e->user_data);
			menu->stop();
			lv_event_send(menu->getLvObj(), LV_EVENT_CANCEL, nullptr);
		}, LV_EVENT_CANCEL, this);
	}

	arrowLeft = lv_img_create(obj);
	lv_obj_set_style_text_color(arrowLeft, lv_color_white(), 0);
	lv_img_set_src(arrowLeft, LV_SYMBOL_LEFT);
	lv_obj_align(arrowLeft, LV_ALIGN_LEFT_MID, 0, 0);

	arrowRight = lv_img_create(obj);
	lv_obj_set_style_text_color(arrowRight, lv_color_white(), 0);
	lv_img_set_src(arrowRight, LV_SYMBOL_RIGHT);
	lv_obj_align(arrowRight, LV_ALIGN_RIGHT_MID, 0, 0);

	lv_anim_init(&anim);
	lv_anim_set_playback_time(&anim, 100);
	lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_path_cb(&anim, lv_anim_path_step);
	lv_anim_set_exec_cb(&anim, arrowAnim);
}

void PicMenu::onStart(){
	lv_obj_scroll_to_x(picLayout, 0, LV_ANIM_OFF);
	lv_group_focus_obj(pics.front());
	refreshArrows();
}

void PicMenu::onStop(){
	lv_anim_del(arrowRight, arrowAnim);
	lv_anim_del(arrowLeft, arrowAnim);
}

uint8_t PicMenu::getSelected(){
	return lv_obj_get_index(lv_group_get_focused(inputGroup));;
}

void PicMenu::refreshArrows(){
	size_t i = lv_obj_get_index(lv_group_get_focused(inputGroup));

	if(i == 0){
		lv_anim_del(arrowLeft, arrowAnim);
	}else if(i == NUM_PICS - 1){
		lv_anim_del(arrowRight, arrowAnim);
	}


	if(i > 0 && lv_anim_get(arrowLeft, nullptr) == nullptr){
		lv_anim_del(arrowLeft, arrowAnim);
		lv_anim_del(arrowRight, arrowAnim);

		lv_anim_set_values(&anim, -3, 0);
		lv_anim_set_var(&anim, arrowLeft);
		lv_anim_start(&anim);

		lv_anim_set_values(&anim, 3, 0);
		lv_anim_set_var(&anim, arrowRight);
		lv_anim_start(&anim);
	}

	if(i < NUM_PICS - 1 && lv_anim_get(arrowRight, nullptr) == nullptr){
		lv_anim_del(arrowLeft, arrowAnim);
		lv_anim_del(arrowRight, arrowAnim);

		lv_anim_set_values(&anim, -3, 0);
		lv_anim_set_var(&anim, arrowLeft);
		lv_anim_start(&anim);

		lv_anim_set_values(&anim, 3, 0);
		lv_anim_set_var(&anim, arrowRight);
		lv_anim_start(&anim);
	}
}