#include "ConvoScreen.h"
#include "../User.h"
#include <Input/Input.h>
#include <Pins.hpp>
#include <Loop/LoopManager.h>
#include "../Services/LoRaService.h"
#include "../Services/MessageService.h"

ConvoScreen::ConvoScreen(UID_t uid) : convo(uid){
	Friend fren = Storage.Friends.get(uid);
	profile = fren.profile;

	lv_obj_t* container = lv_obj_create(obj);
	lv_obj_t* user = (new User(container, profile))->getLvObj();
	convoBox = new ConvoBox(container, uid, profile.hue);
	entry = new TextEntry(container);

	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_all(obj, 3, LV_PART_MAIN);
	lv_obj_set_size(container, lv_pct(100), lv_pct(100));

	lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
	lv_obj_set_style_border_color(container, lv_color_white(), LV_PART_MAIN);

	lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_grow(convoBox->getLvObj(), 1);
	/*lv_obj_set_scrollbar_mode(messages, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_ver(messages, 3, 0);
	lv_obj_set_style_pad_hor(messages, 2, 0);
	lv_obj_set_style_pad_row(messages, 2, 0);
	lv_obj_set_style_bg_color(messages, lv_color_hsv_to_rgb(profile.hue, 40, 50), 0);
	lv_obj_set_style_bg_opa(messages, LV_OPA_100, 0);
	lv_obj_set_style_border_color(messages, lv_color_white(), 0);
	lv_obj_set_style_border_opa(messages, LV_OPA_100, 0);
	lv_obj_set_style_border_width(messages, 1, 0);*/

	lv_obj_set_style_bg_opa(entry->getLvObj(), LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(entry->getLvObj(), lv_color_white(), LV_PART_MAIN);

	lv_obj_set_style_border_width(user, 0, 0);

	entry->setTextColor(lv_color_hex(0x8e478c));
	entry->setPlaceholder("...");

	lv_obj_set_style_pad_hor(entry->getLvObj(), 2, 0);
	lv_obj_set_style_pad_top(entry->getLvObj(), 1, 0);

	lv_obj_add_event_cb(entry->getLvObj(), [](lv_event_t* e){
		auto* screen = static_cast<ConvoScreen*>(e->user_data);
		screen->send();
	}, EV_ENTRY_DONE, this);

	lv_obj_add_event_cb(entry->getLvObj(), [](lv_event_t* e){
		auto* screen = static_cast<ConvoScreen*>(e->user_data);
		screen->entry->stop();
	}, EV_ENTRY_CANCEL, this);

	lv_obj_add_event_cb(convoBox->getLvObj(), [](lv_event_t* e){
		auto* screen = static_cast<ConvoScreen*>(e->user_data);
		auto* msgEl = static_cast<ConvoMessage*>(e->param);
		auto& msg = msgEl->getMsg();

		std::vector<ContextMenu::Option> options;
		if(!msg.received && msg.outgoing){
			options.push_back({ "Resend message", 0 });
		}
		options.push_back({ "Delete message", 1 });

		screen->selectedMessage = msg;
		screen->convoBox->deselect();
		screen->menuResend->setOptions(options);
		screen->menuResend->start();
	}, EV_CONVOBOX_MSG_SELECTED, this);

	menuResend = new ContextMenu(this, {
			{ "Resend message", 0 }
	});

	lv_obj_add_event_cb(menuResend->getLvObj(), [](lv_event_t* e){
		auto* screen = static_cast<ConvoScreen*>(e->user_data);

		const Message& msg = screen->selectedMessage;
		if(msg.uid == 0) return;

		const auto& option = screen->menuResend->getSelected();
		if(option.value == 0 && msg.outgoing && !msg.received){
			Messages.resend(screen->convo, screen->selectedMessage.uid);
		}else if(option.value == 1){
			if(Messages.deleteMessage(screen->convo, msg.uid)){
				screen->convoBox->removeMessage(msg.uid);
			}
		}

		screen->selectedMessage = Message();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(menuResend->getLvObj(), [](lv_event_t* e){
		auto* screen = static_cast<ConvoScreen*>(e->user_data);
		screen->selectedMessage = Message();
	}, LV_EVENT_CANCEL, this);

	picMenu = new PicMenu(this);

	menuConvo = new ContextMenu(this, {
			{ "Memes", 0 }
	});

	lv_obj_add_event_cb(menuConvo->getLvObj(), [](lv_event_t* e){
		auto* screen = static_cast<ConvoScreen*>(e->user_data);
		int16_t option = screen->menuConvo->getSelected().value;

		if(option == 0){
			screen->picMenu->start();
		}
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(picMenu->getLvObj(), [](lv_event_t* e){
		auto* screen = static_cast<ConvoScreen*>(e->user_data);
		uint8_t index = screen->picMenu->getSelected();

		Message msg = Messages.sendPic(screen->convo, index);
		if(msg.uid == 0) return;

		screen->convoBox->addMessage(msg);
	}, LV_EVENT_CLICKED, this);

	lv_group_add_obj(inputGroup, obj);
	lv_group_focus_obj(obj);

	lv_obj_add_event_cb(obj, [](lv_event_t* e){
		auto* screen = static_cast<ConvoScreen*>(e->user_data);
		screen->menuConvo->start();
	}, LV_EVENT_CLICKED, this);
}

void ConvoScreen::onStart(){
	Input::getInstance()->addListener(this);

	if(!loaded){
		convoBox->load();
		loaded = true;
	}
}

void ConvoScreen::onStop(){
	Input::getInstance()->removeListener(this);
}

void ConvoScreen::send(){
	entry->stop();
	std::string text = entry->getText();
	entry->clear();

	Message message = Messages.sendText(convo, text);
	if(message.uid == 0) return;

	convoBox->addMessage(message);
}

void ConvoScreen::buttonPressed(uint i){
	if(i == BTN_ENTER) return;

	if(i != BTN_LEFT && i != BTN_RIGHT && i != BTN_ENTER && i != BTN_BACK){
		if(entry->isActive() || picMenu->isActive() || menuResend->isActive() || menuConvo->isActive()) return;


		if(convoBox->isActive()){
			convoBox->deselect();
		}

		entry->start();
		entry->keyPress(i);
		return;
	}

	if(entry->isActive() || convoBox->isActive() || picMenu->isActive() || menuResend->isActive() || menuConvo->isActive()) return;

	if(i == BTN_BACK){
		pop();
		return;
	}

	if(i == BTN_LEFT || i == BTN_RIGHT){
		lv_event_send(convoBox->getLvObj(), LV_EVENT_CLICKED, nullptr);
		return;
	}
}

