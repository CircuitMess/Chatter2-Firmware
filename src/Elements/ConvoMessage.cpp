#include "ConvoMessage.h"
#include <Arduino.h>
#include "../Fonts/font.h"
#include "../Interface/Pics.h"

ConvoMessage::ConvoMessage(lv_obj_t* parent, const Message& msg, uint16_t bgColor) : LVObject(parent), msg(msg){
	bool outgoing = msg.outgoing;
	bool delivered = msg.received;

	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(obj, outgoing ? LV_FLEX_ALIGN_END : LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_width(obj, lv_pct(100));
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_style_pad_gap(obj, 1, 0);
	lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	if(msg.getType() == Message::TEXT){
		label = lv_label_create(obj);
		lv_label_set_text(label, msg.getText().c_str());
		lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
		lv_obj_set_style_text_font(label, &pixelbasic7, 0);
	}else if(msg.getType() == Message::PIC){
		Pic pic = Pics[msg.getPic()];
		label = pic.create(obj);
	}

	if(outgoing){
		deliveredIndicator = lv_obj_create(obj);
		lv_obj_set_size(deliveredIndicator, 7, 7);
		lv_obj_set_style_radius(deliveredIndicator, LV_RADIUS_CIRCLE, 0);
		lv_obj_set_style_bg_opa(deliveredIndicator, delivered ? LV_OPA_100 : LV_OPA_0, 0);
		lv_obj_set_style_bg_color(deliveredIndicator, lv_color_hsv_to_rgb(bgColor, 70, 90), 0);
		lv_obj_set_style_border_opa(deliveredIndicator, LV_OPA_100, 0);
		lv_obj_set_style_border_color(deliveredIndicator, lv_color_hsv_to_rgb(bgColor, 70, 90), 0);
		lv_obj_set_style_border_width(deliveredIndicator, 2, 0);
	}

	lv_obj_set_height(label, LV_SIZE_CONTENT);
	lv_obj_update_layout(obj);

	lv_obj_set_scrollbar_mode(label, LV_SCROLLBAR_MODE_OFF);

	lv_style_init(&defaultStyle);
	lv_style_set_bg_opa(&defaultStyle, LV_OPA_100);
	lv_style_set_text_line_space(&defaultStyle, 3);
	lv_style_set_text_color(&defaultStyle, outgoing ? lv_color_hsv_to_rgb(bgColor, 70, 90) : lv_color_white());
	lv_style_set_bg_color(&defaultStyle, outgoing ? lv_color_hsv_to_rgb(0, 0, 100) : lv_color_hsv_to_rgb(bgColor, 60, 85));
	lv_style_set_radius(&defaultStyle, 3);
	lv_style_set_width(&defaultStyle, lv_pct(60));
	lv_style_set_pad_all(&defaultStyle, 2);

	lv_style_init(&focusedStyle);
	lv_style_set_bg_color(&focusedStyle, outgoing ? lv_color_hsv_to_rgb(0, 0, 90) : lv_color_hsv_to_rgb(bgColor, 50, 60));

	lv_obj_add_style(label, &defaultStyle, LV_STATE_DEFAULT | LV_PART_MAIN);
	lv_obj_add_style(label, &focusedStyle, LV_STATE_FOCUSED | LV_PART_MAIN);

	//focus forwarding to label child
	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		auto* msg = static_cast<ConvoMessage*>(event->user_data);
		lv_obj_add_state(msg->label, LV_STATE_FOCUSED);
	}, LV_EVENT_FOCUSED, this);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		auto* msg = static_cast<ConvoMessage*>(event->user_data);
		lv_obj_clear_state(msg->label, LV_STATE_FOCUSED);
	}, LV_EVENT_DEFOCUSED, this);
}

void ConvoMessage::setDelivered(bool delivered){
	if(!msg.outgoing) return;

	msg.received = delivered;
	lv_obj_set_style_bg_opa(deliveredIndicator, delivered ? LV_OPA_100 : LV_OPA_0, 0);
	lv_obj_invalidate(obj);
}

const Message& ConvoMessage::getMsg() const{
	return msg;
}

void ConvoMessage::clearFocus(){
	lv_obj_clear_state(obj, LV_STATE_FOCUSED);
	lv_obj_clear_state(label, LV_STATE_FOCUSED);
}

void ConvoMessage::setHue(uint16_t hue){
	if(msg.outgoing){
		lv_obj_set_style_bg_color(deliveredIndicator, lv_color_hsv_to_rgb(hue, 70, 90), 0);
		lv_obj_set_style_border_color(deliveredIndicator, lv_color_hsv_to_rgb(hue, 70, 90), 0);
	}
	lv_style_set_text_color(&defaultStyle, msg.outgoing ? lv_color_hsv_to_rgb(hue, 70, 90) : lv_color_white());
	lv_style_set_bg_color(&defaultStyle, msg.outgoing ? lv_color_hsv_to_rgb(0, 0, 100) : lv_color_hsv_to_rgb(hue, 60, 85));
	lv_style_set_bg_color(&focusedStyle, msg.outgoing ? lv_color_hsv_to_rgb(0, 0, 90) : lv_color_hsv_to_rgb(hue, 50, 60));
	lv_obj_invalidate(obj);
}

ConvoMessage::~ConvoMessage(){
	lv_style_reset(&defaultStyle);
	lv_style_reset(&focusedStyle);
}
