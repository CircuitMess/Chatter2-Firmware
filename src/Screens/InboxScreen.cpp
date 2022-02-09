#include "InboxScreen.h"
#include "../UserWithMessage.h"
#include "../font.h"
#include "../Model/Convo.hpp"
#include "../Storage/Storage.h"
#include "ConvoScreen.h"
#include "../ListItem.h"

InboxScreen::InboxScreen() : LVScreen(), apop(this){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);

	auto listItem = new ListItem(obj,"New conversation",1);
	lv_group_add_obj(inputGroup, listItem->getLvObj());
	lv_obj_add_flag(listItem->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	std::vector<UID_t> frens = Storage.Friends.all();
	params.reserve(frens.size());

	for(UID_t uid : frens){
		if(uid == ESP.getEfuseMac()) continue;

		Friend fren = Storage.Friends.get(uid);
		if(fren.uid == 0) continue;

		std::string text = "";
		Message msg = Messages.getLastMessage(uid);
		if(msg.uid != 0){
			if(msg.getType() == Message::TEXT){
				text = msg.getText();
			}else if(msg.getType() == Message::PIC){
				text = "Meme";
			}
		}

		params.push_back({ uid, this });

		auto user = new UserWithMessage(obj, fren, text);
		lv_group_add_obj(inputGroup, user->getLvObj());
		lv_obj_add_flag(user->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);

		lv_obj_add_event_cb(user->getLvObj(), [](lv_event_t* event){
			LaunchParams* params = static_cast<LaunchParams*>(event->user_data);
			params->ctx->openConvo(params->uid);
		}, LV_EVENT_CLICKED, &params.back());
		userElements.push_back(user);
	}
}

void InboxScreen::openConvo(UID_t uid){
	auto screen = new ConvoScreen(uid);
	push(screen);
}

void InboxScreen::onStarting(){
	for(auto user: userElements){
		Message msg = Messages.getLastMessage(user->getUID());
		if(msg.getType() == Message::TEXT){
			user->setText(msg.getText());
		}else if(msg.getType() == Message::PIC){
			user->setText("Meme");
		}
	}
}

void InboxScreen::onStart(){
	apop.start();
}

void InboxScreen::onStop(){
	apop.stop();
}
