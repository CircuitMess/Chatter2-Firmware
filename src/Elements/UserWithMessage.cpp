#include "UserWithMessage.h"
#include "../Fonts/font.h"
#include "../Storage/Storage.h"

UserWithMessage::UserWithMessage(lv_obj_t* parent, const Friend &fren, const std::string& text) : User(parent, fren){
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

	//new layout for name and message
	lv_obj_t* nameMessageLayout = lv_obj_create(obj);
	lv_obj_set_layout(nameMessageLayout, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(nameMessageLayout, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(nameMessageLayout, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_flex_grow(nameMessageLayout, 1);
	lv_obj_set_parent(name, nameMessageLayout);
	lv_obj_set_height(nameMessageLayout, 25);

	//message label
	message = lv_label_create(nameMessageLayout);
	lv_label_set_long_mode(message, LV_LABEL_LONG_DOT);
	lv_obj_set_style_opa(message, LV_OPA_90, 0);
	lv_obj_set_style_text_color(message, lv_color_white(), 0);
	lv_label_set_text(message, text.c_str());
	lv_obj_set_style_text_font(message, &pixelbasic7, 0);
	lv_obj_set_style_pad_top(message, 1, 0);

	lv_obj_set_flex_grow(name, 1);
	lv_obj_set_flex_grow(message, 1);
	lv_obj_set_width(message, lv_pct(100));
	lv_obj_set_width(name, lv_pct(100));

	notif = lv_img_create(obj);
	lv_img_set_src(notif, "S:/Unread.bin");
	lv_obj_add_flag(notif, LV_OBJ_FLAG_FLOATING);
	lv_obj_align(notif, LV_ALIGN_TOP_RIGHT, 2, 0);

	lv_obj_set_style_img_recolor_opa(notif, LV_OPA_COVER, 0);
	lv_obj_set_style_img_recolor(notif, lv_color_make(255, 230, 0), 0);

	Convo convo = Storage.Convos.get(frenUID);
	if(convo.uid == 0 || !convo.unread){
		lv_obj_add_flag(notif, LV_OBJ_FLAG_HIDDEN);
	}

	if(text.empty()){
		updateText();
	}

	Messages.addReceivedListener(this);
	Messages.addUnreadListener(this);
}

UserWithMessage::~UserWithMessage(){
	Messages.removeReceivedListener(this);
	Messages.removeUnreadListener(this);
}

void UserWithMessage::msgReceived(const Message &message){
	if(message.convo != frenUID) return;

	if(message.getType() == Message::TEXT){
		lv_label_set_text(this->message, message.getText().c_str());
	}else if(message.getType() == Message::PIC){
		lv_label_set_text(this->message, "Meme");
	}
}

void UserWithMessage::setText(const std::string& text){
	lv_label_set_text(message, text.c_str());
}

void UserWithMessage::updateText(){
	std::string text = "";

	Message msg = Messages.getLastMessage(frenUID);
	if(msg.uid != 0){
		if(msg.getType() == Message::TEXT){
			text = msg.getText();
		}else if(msg.getType() == Message::PIC){
			text = "Meme";
		}
	}

	setText(text);
}

void UserWithMessage::onUnread(bool unread){
	Convo convo = Storage.Convos.get(frenUID);

	if(convo.uid == 0 || !convo.unread){
		lv_obj_add_flag(notif, LV_OBJ_FLAG_HIDDEN);
	}else{
		lv_obj_clear_flag(notif, LV_OBJ_FLAG_HIDDEN);
	}
}
