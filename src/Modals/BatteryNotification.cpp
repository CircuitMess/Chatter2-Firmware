#include "BatteryNotification.h"
#include "../Fonts/font.h"
#include <Input/Input.h>
#include <Chatter.h>
#include "../InputChatter.h"
#include "../Services/SleepService.h"

BatteryNotification::BatteryNotification(LVScreen* parent, BatteryNotification::Type type) : LVModal(parent){
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
	lv_obj_set_style_pad_gap(obj, 5, 0);
	lv_obj_set_style_pad_all(obj, 7, 0);

	lv_obj_set_style_border_width(obj, 2, 0);
	lv_obj_set_style_border_color(obj, lv_color_white(), 0);
	lv_obj_set_style_border_opa(obj, LV_OPA_100, 0);

	lv_obj_set_style_bg_color(obj, lv_color_make(86, 64, 100), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);

	auto img = lv_img_create(obj);

	auto label = lv_label_create(obj);
	lv_obj_set_style_text_font(label, &pixelbasic7, 0);
	lv_obj_set_style_text_color(label, lv_color_white(), 0);
	lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(label, LV_ALIGN_CENTER, 0);

	lv_timer_t* timer;
	switch(type){
		case WARNING:
			lv_img_set_src(img, "S:/Battery/1.bin");
			lv_label_set_text(label, "Warning!\n\nBatteries low!");

			lv_group_add_obj(inputGroup, obj);


			lv_obj_add_event_cb(obj, [](lv_event_t* event){
				auto modal = static_cast<BatteryNotification*>(event->user_data);
				if(!modal->isActive()) return;
				lv_group_set_editing((lv_group_t*)lv_obj_get_group(lv_event_get_target(event)), false);
				modal->stop();
			}, LV_EVENT_KEY, this);

			lv_obj_add_event_cb(obj, [](lv_event_t* event){
				auto modal = static_cast<BatteryNotification*>(event->user_data);
				if(!modal->isActive()) return;
				lv_group_set_editing((lv_group_t*)lv_obj_get_group(lv_event_get_target(event)), false);
				modal->stop();
			}, LV_EVENT_PRESSED, this);

			lv_group_focus_obj(obj);
			lv_group_set_editing(inputGroup, true);

			timer = lv_timer_create([](lv_timer_t* timer){
				auto modal = static_cast<BatteryNotification*>(timer->user_data);
				if(!modal->isActive()) return;
				lv_group_set_editing((lv_group_t*)lv_obj_get_group(modal->getLvObj()), false);
				modal->stop();
			}, 5000, this);
			break;

		case SHUTDOWN:
			lv_img_set_src(img, "S:/Battery/0.bin");
			lv_label_set_text(label, "Battery exhausted!\n\nShutting down.");

			timer = lv_timer_create([](lv_timer_t* timer){
				Chatter.fadeOut();
				Sleep.turnOff();
			}, 5000, this);
			break;
	}
//	lv_timer_enable(true);
	lv_timer_set_repeat_count(timer, 1);
	lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
}

void BatteryNotification::onStart(){
	Input::getInstance()->maskAll();
	Input::getInstance()->removeMask(reinterpret_cast<InputChatter*>(InputChatter::getInstance()));
}

void BatteryNotification::onStop(){
	Input::getInstance()->unmaskAll();
}
