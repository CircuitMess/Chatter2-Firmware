#include "LockScreen.h"
#include "../Storage/Storage.h"
#include "../Fonts/font.h"
#include "../Services/SleepService.h"
#include <Input/Input.h>
#include <Pins.hpp>

LockScreen* LockScreen::instance = nullptr;

LockScreen::LockScreen() : LVScreen(){
	lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);

	container = lv_obj_create(obj);
	lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
	lv_obj_set_layout(container, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_all(container, 3, 0);

	slide = new UnlockSlide(obj, [this](){
		stop();
		instance = nullptr;

		auto parent = getParent();
		if(parent){
			parent->start(true, LV_SCR_LOAD_ANIM_MOVE_BOTTOM);
		}

		lv_obj_del_delayed(obj, 1000);
	});

	lv_obj_add_flag(slide->getLvObj(), LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_align(slide->getLvObj(), LV_ALIGN_BOTTOM_MID);

	instance = this;
}

LockScreen::~LockScreen(){

}

void LockScreen::activate(LVScreen* parent){
	printf("Lock screen activate\n");

	parent->stop();

	if(instance){
		printf("Git instance\n");

		if(parent != instance){
			printf("Not this one\n");
			instance->setParent(parent);
		}

		instance->start();
	}else{
		printf("Not instance, creating new\n");
		auto screen = new LockScreen();
		screen->setParent(parent);
		screen->start();
	}
}

void LockScreen::onStarting(){
	loadUnread();
	slide->reset();
	printf("On starting\n");
}

void LockScreen::onStart(){
	Input::getInstance()->addListener(this);
	Messages.addUnreadListener(this);
	printf("On start\n");
}

void LockScreen::onStop(){
	slide->stop();
	Input::getInstance()->removeListener(this);
	Messages.removeUnreadListener(this);
	printf("On stop\n");
}

void LockScreen::buttonPressed(uint i){
	if(i == BTN_BACK){
		Sleep.enterSleep();
		return;
	}else if(i != BTN_R){
		slide->shake();
		return;
	}

	slide->start();
}

void LockScreen::buttonReleased(uint i){
	if(i != BTN_R) return;

	slide->stop();
	slide->reset();
}

void LockScreen::onUnread(bool unread){
	unreads.clear();
	if(noUnreads){
		lv_obj_del(noUnreads);
		noUnreads = nullptr;
	}

	if(unread){
		loadUnread();
	}else{
		createNoUnreads();
	}
}

void LockScreen::loadUnread(){
	unreads.clear();

	auto convos = Storage.Convos.all();
	for(auto convo : convos){
		if(!Storage.Convos.get(convo).unread) continue;

		auto fren = Storage.Friends.get(convo);

		auto el = std::make_unique<UserWithMessage>(container, fren);
		unreads.push_back(std::move(el));
	}

	if(unreads.empty()){
		createNoUnreads();
	}
}

void LockScreen::createNoUnreads(){
	if(noUnreads){
		lv_obj_del(noUnreads);
		noUnreads = nullptr;
	}

	noUnreads = lv_label_create(container);
	lv_label_set_text(noUnreads, "You have no new messages.");
	lv_obj_set_style_text_font(noUnreads, &pixelbasic7, 0);
	lv_obj_set_style_text_color(noUnreads, lv_color_make(200, 200, 200), 0);
}
