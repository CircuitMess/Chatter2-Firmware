#include "Convo.h"
#include "ConvoMessage.h"
#include "User.h"
#include <Input/Input.h>
#include <Pins.hpp>
#include <Loop/LoopManager.h>
#include "Services/LoRaService.h"

Convo::Convo(const Profile& profile) : profile(profile){
	lv_obj_t* container = lv_obj_create(obj);
	new User(container, profile);
	messages = lv_obj_create(container);
	entry = new TextEntry(container);

	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_all(obj, 4, LV_PART_MAIN);
	lv_obj_set_size(container, lv_pct(100), lv_pct(100));

	lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
	lv_obj_set_style_border_color(container, lv_color_white(), LV_PART_MAIN);
	/*lv_obj_set_style_bg_img_src(obj, "S:/bg.bin", LV_PART_MAIN);
	lv_obj_set_style_bg_img_opa(obj, LV_OPA_100, LV_PART_MAIN);*/
	lv_obj_set_style_bg_color(obj, lv_color_hex(0x39314b), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);

	lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_layout(messages, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(messages, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_grow(messages, 1);
	lv_obj_set_width(messages, lv_pct(100));
	lv_obj_set_scrollbar_mode(messages, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_ver(messages, 3, 0);
	lv_obj_set_style_pad_hor(messages, 2, 0);
	lv_obj_set_style_pad_row(messages, 2, 0);
	lv_obj_set_style_bg_color(messages, lv_color_hsv_to_rgb(profile.color, 40, 50), 0);
	lv_obj_set_style_bg_opa(messages, LV_OPA_100, 0);
	lv_obj_set_style_border_color(messages, lv_color_white(), 0);
	lv_obj_set_style_border_opa(messages, LV_OPA_100, 0);
	lv_obj_set_style_border_width(messages, 1, 0);

	lv_obj_set_style_bg_opa(entry->getLvObj(), LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(entry->getLvObj(), lv_color_white(), LV_PART_MAIN);

	entry->setTextColor(lv_color_hex(0x8e478c));
	entry->setPlaceholder("...");

	lv_obj_set_style_pad_hor(entry->getLvObj(), 2, 0);
	lv_obj_set_style_pad_top(entry->getLvObj(), 1, 0);
}

void Convo::onStart(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);
}

void Convo::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void Convo::buttonPressed(uint i){
	if(i == BTN_ENTER){
		if(entry->isActive()){
			new ConvoMessage(messages, entry->getText().c_str(), true, 0);
			// LoRa.send(0, LoRaPacket::MSG, (void*) entry->getText().c_str(), entry->getText().size() + 1);

			entry->stop();
			entry->clear();
		}

		return;
	}

	if(i == BTN_BACK){
		if(!entry->isActive()){
			pop();
			return;
		}

		return;
	}

	if(i != BTN_LEFT && i != BTN_RIGHT){
		if(entry->isActive()) return;
		entry->start();
		entry->keyPress(i);
	}
}

void Convo::loop(uint micros){
	ReceivedPacket<MessagePacket> msg = LoRa.getMessage();
	if(msg.content == nullptr) return;

	if(msg.content->type == MessagePacket::TEXT){
		TextMessage* txt = static_cast<TextMessage*>(msg.content);
		printf("Got message: %s\n", txt->text.c_str());
		new ConvoMessage(messages, txt->text.c_str(), false, profile.color);
	}

	delete msg.content;
}
