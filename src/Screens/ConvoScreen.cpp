#include "ConvoScreen.h"
#include "../Elements/User.h"
#include <Input/Input.h>
#include <Pins.hpp>
#include <Loop/LoopManager.h>
#include "../Services/LoRaService.h"
#include "../Fonts/font.h"
#include "../Services/BuzzerService.h"

ConvoScreen::ConvoScreen(UID_t uid) : convo(uid){
	fren = Storage.Friends.get(uid);
	Profile profile = fren.profile;

	lv_obj_set_style_pad_all(obj, 3, LV_PART_MAIN);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_t* container = lv_obj_create(obj);
	lv_obj_set_size(container, lv_pct(100), lv_pct(100));
	lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
	lv_obj_set_style_border_color(container, lv_color_white(), LV_PART_MAIN);
	lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);

	lv_obj_t* user = (new User(container, fren))->getLvObj();
	convoBox = new ConvoBox(container, uid, profile.hue);
	textEntry = new TextEntry(container, "", 60);
	textEntry->showCaps(true);

	lv_obj_set_style_border_width(user, 1, 0);
	lv_obj_set_style_border_color(user, lv_color_white(), 0);
	lv_obj_set_style_border_side(user, LV_BORDER_SIDE_BOTTOM, 0);

	lv_obj_set_flex_grow(convoBox->getLvObj(), 1);

	lv_obj_set_style_bg_opa(textEntry->getLvObj(), LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(textEntry->getLvObj(), lv_color_white(), LV_PART_MAIN);
	lv_obj_set_style_pad_hor(textEntry->getLvObj(), 2, 0);
	lv_obj_set_style_pad_top(textEntry->getLvObj(), 1, 0);
	lv_obj_set_style_text_font(textEntry->getLvObj(), &lv_font_montserrat_14, 0);
	textEntry->setTextColor(lv_color_black());

	picMenu = new PicMenu(this);

	menuMessage = new ContextMenu(this);


	lv_obj_add_event_cb(textEntry->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->textEntryConfirm();
	}, EV_ENTRY_DONE, this);

	lv_obj_add_event_cb(textEntry->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->textEntryCancel();
	}, EV_ENTRY_CANCEL, this);

	lv_obj_add_event_cb(textEntry->getLvObj(), [](lv_event_t* e){
		auto screen = static_cast<ConvoScreen*>(e->user_data);
		screen->textEntryLR();
	}, EV_ENTRY_LR, this);

	lv_obj_add_event_cb(convoBox->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->convoBoxEnter();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(convoBox->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->convoBoxExit();
	}, EV_ENTRY_CANCEL, this);

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

	lv_obj_add_event_cb(picMenu->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->picMenuSelected();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(picMenu->getLvObj(), [](lv_event_t* e){
		static_cast<ConvoScreen*>(e->user_data)->picMenuCancel();
	}, LV_EVENT_CANCEL, this);

}

void ConvoScreen::onStart(){
	Input::getInstance()->addListener(this);
	setButtonHoldTime(BTN_R, 500);

	Messages.markRead(convo);

	if(!loaded){
		convoBox->load();
		loaded = true;
	}

	textEntry->start();
}

void ConvoScreen::onStop(){
	Input::getInstance()->removeListener(this);
	textEntry->stop();
	Buzz.setNoBuzzUID(ESP.getEfuseMac());
}

void ConvoScreen::buttonPressed(uint i){
	if(i == BTN_ENTER || i == BTN_LEFT || i == BTN_RIGHT) return;

	if(i != BTN_BACK){
		if(textEntry->isActive() || picMenu->isActive() || menuMessage->isActive()) return;


		if(convoBox->isActive()){
			convoBox->deselect();
		}

		textEntry->start();
		textEntry->keyPress(i);
		return;
	}

	if(textEntry->isActive() || convoBox->isActive() || picMenu->isActive() || menuMessage->isActive()) return;

	if(i == BTN_BACK){
		if(textEntry->isActive() && !textEntry->getText().empty()) return;
		pop();
		return;
	}
}

void ConvoScreen::buttonHeld(uint i){
	if(i != BTN_R) return;
	if(picMenu->isActive() || menuMessage->isActive()) return;

	convoBox->deselect();
	textEntry->stop();

	picMenu->start();
}

void ConvoScreen::buttonReleased(uint i){
	if(i != BTN_LEFT && i != BTN_RIGHT) return;

	if(textEntry->isActive() || picMenu->isActive() || menuMessage->isActive() || convoBox->isActive()) return;

	lv_async_call([](void* user_data){
		auto cBox = static_cast<lv_obj_t*>(user_data);
		lv_event_send(cBox, LV_EVENT_CLICKED, nullptr);
	}, convoBox->getLvObj());
}

void ConvoScreen::sendMessage(){
	std::string text = textEntry->getText();
	if(text == "") return;

	textEntry->clear();

	Message message = Messages.sendText(convo, text);
	if(message.uid == 0) return;

	lv_timer_handler();

	convoBox->addMessage(message);
}

void ConvoScreen::textEntryConfirm(){
	sendMessage();
}

void ConvoScreen::textEntryCancel(){
	textEntry->stop();

	if(textEntry->getText().empty()){
		pop();
		return;
	}
}

void ConvoScreen::textEntryLR(){
	std::string text = textEntry->getText();
	if(!text.empty()) return;
	textEntry->stop();
	lv_event_send(convoBox->getLvObj(), LV_EVENT_CLICKED, nullptr);
}

void ConvoScreen::convoBoxEnter(){
	textEntry->stop();
}

void ConvoScreen::convoBoxExit(){
	textEntry->start();
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
}

void ConvoScreen::menuMessageSelected(){
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
	textEntry->start();
}

void ConvoScreen::menuMessageCancel(){
	selectedMessage = Message();
	textEntry->start();
}

void ConvoScreen::picMenuSelected(){
	uint8_t index = picMenu->getSelected();

	Message msg = Messages.sendPic(convo, index);
	if(msg.uid == 0) return;

	lv_timer_handler();

	convoBox->addMessage(msg);

	textEntry->start();
}

void ConvoScreen::picMenuCancel(){
	textEntry->start();
}

void ConvoScreen::onStarting(){
	Buzz.setNoBuzzUID(convo);
}
