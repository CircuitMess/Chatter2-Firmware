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

	lv_obj_set_style_pad_all(obj, 3, LV_PART_MAIN);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_t* container = lv_obj_create(obj);
	lv_obj_set_size(container, lv_pct(100), lv_pct(100));
	lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
	lv_obj_set_style_border_color(container, lv_color_white(), LV_PART_MAIN);
	lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);

	lv_obj_t* user = (new User(container, profile))->getLvObj();
	convoBox = new ConvoBox(container, uid, profile.hue);
	textEntry = new TextEntry(container);

	lv_obj_set_style_border_width(user, 0, 0);

	lv_obj_set_flex_grow(convoBox->getLvObj(), 1);

	lv_obj_set_style_bg_opa(textEntry->getLvObj(), LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(textEntry->getLvObj(), lv_color_white(), LV_PART_MAIN);
	lv_obj_set_style_pad_hor(textEntry->getLvObj(), 2, 0);
	lv_obj_set_style_pad_top(textEntry->getLvObj(), 1, 0);
	textEntry->setTextColor(lv_color_hex(0x8e478c));
	textEntry->setPlaceholder("...");

	picMenu = new PicMenu(this);

	menuMessage = new ContextMenu(this);

	menuConvo = new ContextMenu(this, {
			{ "Memes", 0 }
	});


	lv_obj_add_event_cb(textEntry->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->textEntryConfirm();
	}, EV_ENTRY_DONE, this);

	lv_obj_add_event_cb(textEntry->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->textEntryCancel();
	}, EV_ENTRY_CANCEL, this);

	lv_obj_add_event_cb(convoBox->getLvObj(), [](lv_event_t* e){
		const auto& msg = static_cast<ConvoMessage*>(e->param)->getMsg();
		static_cast<ConvoScreen*>(e->user_data)->convoBoxEnter();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(convoBox->getLvObj(), [](lv_event_t* e){
		const auto& msg = static_cast<ConvoMessage*>(e->param)->getMsg();
		static_cast<ConvoScreen*>(e->user_data)->convoBoxExit();
	}, LV_EVENT_READY, this);

	lv_obj_add_event_cb(convoBox->getLvObj(), [](lv_event_t* e){
		const auto& msg = static_cast<ConvoMessage*>(e->param)->getMsg();
		static_cast<ConvoScreen*>(e->user_data)->messageSelected(msg);
	}, EV_CONVOBOX_MSG_SELECTED, this);

	lv_obj_add_event_cb(menuMessage->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->menuMessageSelected();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(menuMessage->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->menuMessageCancel();
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(menuConvo->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->menuConvoSelected();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(menuConvo->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->menuConvoCancel();
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(picMenu->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->picMenuSelected();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(picMenu->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->picMenuCancel();
	}, LV_EVENT_CANCEL, this);


	auto lrClick = [](lv_event_t* e){
		auto* screen = static_cast<ConvoScreen*>(e->user_data);
		screen->textEntry->defocus();
		screen->menuConvo->start();
	};

	for(int i = 0; i < 2; i++){
		lv_obj_t* helper = lv_obj_create(obj);
		lv_obj_add_flag(helper, LV_OBJ_FLAG_FLOATING);
		lv_obj_clear_flag(helper, LV_OBJ_FLAG_CLICK_FOCUSABLE);
		lv_obj_clear_flag(helper, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
		lv_obj_clear_flag(helper, LV_OBJ_FLAG_CHECKABLE);
		lv_obj_clear_flag(helper, LV_OBJ_FLAG_SCROLLABLE);

		lv_group_add_obj(inputGroup, helper);
		lv_obj_add_event_cb(helper, lrClick, LV_EVENT_CLICKED, this);
	}

	lv_group_focus_obj(lv_obj_get_child(obj, -1));

	inputGroup->user_data = this;
	lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
		auto* screen = static_cast<ConvoScreen*>(group->user_data);
		lv_event_send(screen->convoBox->getLvObj(), LV_EVENT_CLICKED, nullptr);
	});
}

void ConvoScreen::onStart(){
	Input::getInstance()->addListener(this);

	if(!loaded){
		convoBox->load();
		loaded = true;
	}

	textEntry->focus();
}

void ConvoScreen::onStop(){
	Input::getInstance()->removeListener(this);
	textEntry->defocus();
}

void ConvoScreen::buttonPressed(uint i){
	if(i == BTN_ENTER || i == BTN_LEFT || i == BTN_RIGHT) return;

	if(i != BTN_BACK){
		if(textEntry->isActive() || picMenu->isActive() || menuMessage->isActive() || menuConvo->isActive()) return;


		if(convoBox->isActive()){
			convoBox->deselect();
		}

		textEntry->start();
		textEntry->keyPress(i);
		return;
	}

	if(textEntry->isActive() || convoBox->isActive() || picMenu->isActive() || menuMessage->isActive() || menuConvo->isActive()) return;

	if(i == BTN_BACK){
		pop();
		return;
	}
}

void ConvoScreen::textEntryConfirm(){
	textEntry->stop();
	std::string text = textEntry->getText();
	textEntry->clear();
	textEntry->focus();

	Message message = Messages.sendText(convo, text);
	if(message.uid == 0) return;

	convoBox->addMessage(message);
}

void ConvoScreen::textEntryCancel(){
	textEntry->stop();
	textEntry->focus();
}

void ConvoScreen::convoBoxEnter(){
	textEntry->defocus();
}

void ConvoScreen::convoBoxExit(){
	textEntry->focus();
}

void ConvoScreen::messageSelected(const Message& msg){
	convoBox->deselect();
	selectedMessage = msg;

	std::vector<ContextMenu::Option> options;
	if(!msg.received && msg.outgoing){
		options.push_back({ "Resend message", 0 });
	}
	options.push_back({ "Delete message", 1 });

	menuMessage->setOptions(options);
	menuMessage->start();

	textEntry->defocus();
}

void ConvoScreen::menuMessageSelected(){
	textEntry->focus();

	if(selectedMessage.uid == 0) return;

	const auto& option = menuMessage->getSelected();

	if(option.value == 0 && selectedMessage.outgoing && !selectedMessage.received){
		Messages.resend(convo, selectedMessage.uid);
	}else if(option.value == 1){
		if(Messages.deleteMessage(convo, selectedMessage.uid)){
			convoBox->removeMessage(selectedMessage.uid);
		}
	}

	selectedMessage = Message();
}

void ConvoScreen::menuMessageCancel(){
	selectedMessage = Message();
	textEntry->focus();
}

void ConvoScreen::menuConvoSelected(){
	int16_t option = menuConvo->getSelected().value;

	if(option == 0){
		picMenu->start();
	}else{
		textEntry->focus();
	}
}

void ConvoScreen::menuConvoCancel(){
	textEntry->focus();
}

void ConvoScreen::picMenuSelected(){
	textEntry->focus();

	uint8_t index = picMenu->getSelected();

	Message msg = Messages.sendPic(convo, index);
	if(msg.uid == 0) return;

	convoBox->addMessage(msg);
}

void ConvoScreen::picMenuCancel(){
	textEntry->focus();
}
