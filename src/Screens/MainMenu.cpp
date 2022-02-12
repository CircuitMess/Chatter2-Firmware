#include "MainMenu.h"
#include "../Model/Convo.hpp"
#include <Input/Input.h>
#include <Pins.hpp>
#include "../Services/LoRaService.h"
#include "../Elements/BatteryElement.h"
#include "../Elements/NotificationElement.h"
#include "InboxScreen.h"
#include "FriendsScreen.h"
#include "SettingsScreen.h"
#include "ProfileScreen.h"
#include "../FSLVGL.h"

const MainMenu::Item MainMenu::Items[] = {
		{ "Inbox", "Inbox", -10 },
		{ "Friends", "Friends", 0 },
		{ "Profile", "Profile", 5 },
		{ "Settings", "Settings", 10 },
};

const uint8_t MainMenu::ItemCount = sizeof(Items) / sizeof(Items[0]);

MainMenu::MainMenu() : LVScreen(){
	left = lv_obj_create(obj);
	mid = lv_obj_create(obj);
	right = lv_obj_create(obj);

	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);

	lv_obj_set_width(left, 22);
	lv_obj_set_width(right, 19);
	lv_obj_set_height(left, lv_pct(100));
	lv_obj_set_height(right, lv_pct(100));
	lv_obj_set_height(mid, lv_pct(100));
	lv_obj_set_flex_grow(mid, 1);
	lv_obj_set_scrollbar_mode(mid, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(right, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(left, LV_SCROLLBAR_MODE_OFF);

	lv_obj_set_flex_flow(left, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_flow(mid, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_flow(right, LV_FLEX_FLOW_COLUMN);

	lv_obj_set_flex_align(left, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
	lv_obj_set_flex_align(right, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_flex_align(mid, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	lv_obj_set_style_pad_ver(left, 2, 0);
	lv_obj_set_style_pad_gap(left, 4, 0);
	new BatteryElement(left);
	notif = new NotificationElement(left);

	for(const auto& item : Items){
		lv_obj_t* bigContainer = lv_obj_create(mid);
		lv_obj_t* big = lv_gif_create(bigContainer);
		lv_obj_t* bigLabel = lv_img_create(bigContainer);
		lv_obj_t* small = lv_img_create(right);

		bigContainers.push_back(bigContainer);
		bigs.push_back(big);
		smalls.push_back(small);

		lv_gif_set_src(big, (String("S:/Menu/Big/") + item.icon + ".gif").c_str());
		lv_gif_set_loop(big, LV_GIF_LOOP_ON);
		lv_obj_set_style_pad_bottom(big, 4, 0);

		lv_img_set_src(bigLabel, (String("S:/Menu/Label/") + item.icon + ".bin").c_str());
		lv_img_set_src(small, (String("S:/Menu/Small/") + item.icon + ".bin").c_str());

		lv_obj_set_flex_flow(bigContainer, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_flex_align(bigContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
		lv_obj_set_width(bigContainer, lv_pct(100));
		lv_obj_set_height(bigContainer, lv_pct(100));

		//lv_obj_set_style_translate_y(big, -5, LV_PART_MAIN | LV_STATE_DEFAULT);
		//lv_obj_set_style_translate_y(bigLabel, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_translate_y(bigContainer, item.offset, LV_PART_MAIN | LV_STATE_DEFAULT);

		lv_group_add_obj(inputGroup, bigContainer);
		lv_obj_add_event_cb(bigContainer, [](lv_event_t* e){
			auto* menu = static_cast<MainMenu*>(e->user_data);
			menu->launch();
		}, LV_EVENT_PRESSED, this);
		lv_obj_clear_flag(bigContainer, LV_OBJ_FLAG_CHECKABLE);
		lv_obj_clear_flag(bigContainer, LV_OBJ_FLAG_SCROLLABLE);
	}

	lv_group_set_wrap(inputGroup, false);
	lv_group_focus_obj(bigContainers.front());

	inputGroup->user_data = this;
	lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
		auto* menu = static_cast<MainMenu*>(group->user_data);
		lv_obj_t* focused = lv_group_get_focused(group);
		uint8_t index = lv_obj_get_child_id(focused);
		menu->scrollTo(index);
	});

	arrowUp = lv_img_create(obj);
	arrowDown = lv_img_create(obj);
	lv_img_set_src(arrowUp, "S:/Menu/ArrowUp.bin");
	lv_img_set_src(arrowDown, "S:/Menu/ArrowDown.bin");
	lv_obj_add_flag(arrowUp, LV_OBJ_FLAG_FLOATING);
	lv_obj_add_flag(arrowDown, LV_OBJ_FLAG_FLOATING);
	lv_obj_set_align(arrowUp, LV_ALIGN_TOP_MID);
	lv_obj_set_align(arrowDown, LV_ALIGN_BOTTOM_MID);
	lv_obj_set_style_pad_top(arrowUp, 4, 0);
	lv_obj_set_style_pad_bottom(arrowDown, 4, 0);

	// Initial state
	lv_obj_set_y(arrowUp, -(13 + 2));
	lv_obj_set_y(arrowDown, 13 + 2);

	for(int i = 0; i < ItemCount; i++){
		lv_obj_set_style_translate_x(smalls[i], lv_pct(110), LV_STATE_DEFAULT | LV_PART_MAIN);
	}

	for(int i = 1; i < ItemCount; i++){
		lv_obj_add_flag(bigContainers[i], LV_OBJ_FLAG_HIDDEN);
	}

	lv_obj_scroll_by(mid, 0, 128, LV_ANIM_OFF);
}

void MainMenu::setupAnimations(){
	lv_anim_init(&arrowUpAnim);
	lv_anim_init(&arrowDownAnim);
	lv_anim_set_var(&arrowUpAnim, arrowUp);
	lv_anim_set_var(&arrowDownAnim, arrowDown);
	lv_anim_set_values(&arrowUpAnim, -500, 500);
	lv_anim_set_values(&arrowDownAnim, -500, 500);
	lv_anim_set_path_cb(&arrowUpAnim, lv_anim_path_ease_in_out);
	lv_anim_set_path_cb(&arrowDownAnim, lv_anim_path_ease_in_out);
	lv_anim_set_exec_cb(&arrowUpAnim, MainMenu::arrowFloat);
	lv_anim_set_exec_cb(&arrowDownAnim, MainMenu::arrowFloat);
	lv_anim_set_repeat_count(&arrowUpAnim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_repeat_count(&arrowDownAnim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_playback_time(&arrowUpAnim, 800);
	lv_anim_set_playback_time(&arrowDownAnim, 800);
	lv_anim_set_time(&arrowUpAnim, 800);
	lv_anim_set_time(&arrowDownAnim, 800);
	lv_anim_set_delay(&arrowDownAnim, 800);

	lv_anim_start(&arrowUpAnim);
	lv_anim_start(&arrowDownAnim);

	for(lv_obj_t* small : smalls){
		smallAnims.emplace_back();
		lv_anim_t& anim = smallAnims.back();
		lv_anim_init(&anim);
		lv_anim_set_exec_cb(&anim, ease);
		lv_anim_set_var(&anim, small);
		lv_anim_set_time(&anim, 800);
	}

	lv_anim_init(&arrowHideAnim1);
	lv_anim_set_exec_cb(&arrowHideAnim1, arrowHide);
	lv_anim_set_time(&arrowHideAnim1, 400);

	lv_anim_init(&arrowHideAnim2);
	lv_anim_set_exec_cb(&arrowHideAnim2, arrowHide);
	lv_anim_set_time(&arrowHideAnim2, 400);
}

void MainMenu::arrowHide(void* var, int32_t value){
	lv_obj_t* obj = (lv_obj_t*) var;
	lv_obj_set_y(obj, value);
}

void MainMenu::arrowFloat(void* var, int32_t value){
	lv_obj_t* obj = (lv_obj_t*) var;
	lv_obj_set_x(obj, round((float) value / 100.0f));
}

void IRAM_ATTR MainMenu::ease(void* var, int32_t value){
	lv_obj_t* obj = (lv_obj_t*) var;

	const float amount = 1.1f;
	float v = (float) value / 100.0f;

	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	const float x =  v < 0.5
			   ? (pow(2.0f * v, 2.0f) * ((c2 + 1.0f) * 2.0f * v - c2)) / 2.0f
			   : (pow(2.0f * v - 2.0f, 2.0f) * ((c2 + 1.0f) * (v * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;

	lv_obj_set_style_translate_x(obj, lv_pct(round(x * amount * 100.0f)), LV_STATE_DEFAULT | LV_PART_MAIN);
}

void MainMenu::startAnim(uint8_t index, bool reverse){
	if(index >= smallAnims.size()) return;

	lv_anim_t& anim = smallAnims[index];
	lv_anim_set_values(&anim, reverse * 100, !reverse * 100);
	lv_anim_start(&anim);

	if(reverse){
		lv_gif_stop(bigs[index]);
	}else{
		lv_gif_start(bigs[index]);
	}
}

void MainMenu::onStarting(){
	for(int i = 0; i < ItemCount; i++){
		lv_gif_restart(bigs[i]);
	}

	if(inited){
		for(int i = 0; i < ItemCount; i++){
			lv_obj_set_style_translate_x(smalls[i], selected == i ? lv_pct(110) : 0, LV_STATE_DEFAULT | LV_PART_MAIN);
		}

		lv_obj_set_y(arrowUp, 0);
		lv_obj_set_y(arrowDown, 0);
		if(selected == 0){
			lv_obj_set_y(arrowUp, -(13 + 2));
		}

		if(selected == ItemCount - 1){
			lv_obj_set_y(arrowDown, 13 + 2);
		}

		lv_group_focus_obj(bigs[selected]);
		lv_obj_scroll_to_y(mid, selected * 128, LV_ANIM_OFF);
	}

	setupAnimations();
	notif->start();
}

void MainMenu::onStart(){
	lv_gif_start(bigs[selected]);

	if(!inited){
		for(int i = 1; i < ItemCount; i++){
			startAnim(i, true);
		}

		if(ItemCount > 1){
			lv_anim_set_var(&arrowHideAnim2, arrowDown);
			lv_anim_set_values(&arrowHideAnim2, 13 + 2, 0);
			lv_anim_start(&arrowHideAnim2);
		}

		lv_obj_add_event_cb(mid, [](lv_event_t* e){
			auto* menu = static_cast<MainMenu*>(e->user_data);
			lv_obj_remove_event_cb_with_user_data(menu->mid, nullptr, menu);

			for(int i = 1; i < ItemCount; i++){
				lv_obj_clear_flag(menu->bigContainers[i], LV_OBJ_FLAG_HIDDEN);
			}
		}, LV_EVENT_SCROLL_END, this);

		lv_obj_scroll_to(mid, 0, 0, LV_ANIM_ON);
		inited = true;
	}
}

void MainMenu::onStop(){
	for(int i = 0; i < ItemCount; i++){
		lv_gif_stop(bigs[i]);
		lv_anim_del(smalls[i], ease);
	}

	lv_anim_del(arrowUp, arrowHide);
	lv_anim_del(arrowDown, arrowHide);

	lv_anim_del(arrowUp, arrowFloat);
	lv_anim_del(arrowDown, arrowFloat);

	notif->stop();
}

void MainMenu::scrollTo(uint8_t index){
	if(index > selected){
		selectNext();
	}else if(index < selected){
		selectPrev();
	}
}

void MainMenu::launch(){
	LVScreen* (* screens[])() = {
			[]() -> LVScreen*{ return new InboxScreen(); },
			[]() -> LVScreen*{ return new FriendsScreen(); },
			[]() -> LVScreen*{ return new ProfileScreen(ESP.getEfuseMac(), true); },
			[]() -> LVScreen*{ return new SettingsScreen(); }
	};

	LVScreen* screen = screens[selected]();
	if(screen){
		push(screen);
	}
}

void MainMenu::selectNext(){
	if(selected + 1 >= ItemCount) return;

	startAnim(selected, true);
	selected++;
	startAnim(selected);
	lv_obj_scroll_to(mid, 0, selected * lv_obj_get_height(mid), LV_ANIM_ON);

	if(selected + 1 == ItemCount){
		lv_anim_set_var(&arrowHideAnim1, arrowDown);
		lv_anim_set_values(&arrowHideAnim1, 0, lv_obj_get_height(arrowDown) + 2);
		lv_anim_start(&arrowHideAnim1);
	}

	if(selected == 1){
		lv_anim_set_var(&arrowHideAnim2, arrowUp);
		lv_anim_set_values(&arrowHideAnim2, -(lv_obj_get_height(arrowDown) + 2), 0);
		lv_anim_start(&arrowHideAnim2);
	}
}

void MainMenu::selectPrev(){
	if(selected == 0) return;

	startAnim(selected, true);
	selected--;
	startAnim(selected);
	lv_obj_scroll_to(mid, 0, selected * lv_obj_get_height(mid), LV_ANIM_ON);

	if(selected == 0){
		lv_anim_set_var(&arrowHideAnim1, arrowUp);
		lv_anim_set_values(&arrowHideAnim1, 0, -(lv_obj_get_height(arrowDown) + 2));
		lv_anim_start(&arrowHideAnim1);
	}

	if(selected + 2 == ItemCount){
		lv_anim_set_var(&arrowHideAnim2, arrowDown);
		lv_anim_set_values(&arrowHideAnim2, lv_obj_get_height(arrowDown) + 2, 0);
		lv_anim_start(&arrowHideAnim2);
	}
}
