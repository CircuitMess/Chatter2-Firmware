#include "ConvoBox.h"
#include "ConversationStruct.hpp"
#include "ConvoMessage.h"

const uint8_t bufferLength = 10;
int lastFocused = -1;

ConvoBox::ConvoBox(ConversationStruct* convo, LVScreen* parent) : LVObject(parent->getLvObj()), convo(convo), inputGroup(parent->getInputGroup()),
																  messageView(convo->getMessageView(max((int)convo->messageCount - bufferLength, 0),
																									min((uint)bufferLength, convo->messageCount))){

	inputGroup->user_data = this;

	lv_obj_set_size(obj, lv_pct(100), lv_pct(100));
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_bg_color(obj, lv_color_make(86, 64, 100), 0);

	lv_obj_set_style_pad_gap(obj, 3, 0);
	lv_obj_set_style_pad_all(obj, 1, 0);

	lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
	fillMessages();
	lv_obj_set_style_outline_pad(obj, 1, 0);
	lv_obj_set_style_outline_width(obj, 1, 0);
	lv_obj_set_style_outline_color(obj, lv_color_white(), 0);
	lv_obj_set_style_outline_color(obj, lv_palette_main(LV_PALETTE_RED), LV_STATE_EDITED);
	lv_obj_set_style_outline_opa(obj, LV_OPA_100, 0);

	lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
		lv_obj_t* focused = lv_group_get_focused(group);
		Serial.print("focused cb index ");
		Serial.println(lv_obj_get_index(focused));
		Serial.printf("lastFocused: %d\n", lastFocused);
		delay(5);
		if(lv_obj_get_index(focused) == lastFocused) return;
		if(lv_obj_get_index(focused) == 0 && ((lastFocused == bufferLength - 1) || lastFocused == lv_obj_get_index(focused))){
			Serial.println("load new messages down");
			static_cast<ConvoBox*>(group->user_data)->loadBelow();
			return;
		}else if(lv_obj_get_index(focused) == bufferLength - 1 &&
				 (lastFocused == 0 || lastFocused == lv_obj_get_index(focused))){
			Serial.println("load new messages up");
			static_cast<ConvoBox*>(group->user_data)->loadAbove();
			return;
		}else{
			lv_obj_scroll_to_view(focused, LV_ANIM_ON);
		}
		focused = lv_group_get_focused(group);
		lv_obj_scroll_to_view(focused, LV_ANIM_ON);
		lastFocused = lv_obj_get_index(focused);
	});



	//focus on the last message at the start (if it exists)
	if(convo->messageCount > 0){
		lv_group_focus_obj(lv_obj_get_child(obj, -1));
	}
}

void ConvoBox::fillMessages(){
	Serial.print("child count before deletion: ");
	Serial.println(lv_obj_get_child_cnt(obj));
	delay(5);
	int childCount = lv_obj_get_child_cnt(obj);
	for(int i = 1; i <= childCount; ++i){
		lv_obj_del_async(lv_obj_get_child(obj, -i));
	}
	lv_obj_add_event_cb(obj, [](lv_event_t* e){
		Serial.printf("deleted child: %d\n", lv_obj_get_index(lv_event_get_target(e)));
	}, LV_EVENT_CHILD_DELETED, this);
//	lv_obj_clean(obj);
	Serial.print("child count after deletion: ");
	Serial.println(lv_obj_get_child_cnt(obj));
	delay(5);
	for(int i = 0; i < messageView.count; ++i){
		auto mess = new ConvoMessage(obj, messageView.messages[i].content, messageView.messages[i].sender != convo->person->uid,
									 convo->person->profile.color, true);
		lv_group_add_obj(inputGroup, mess->getLvObj());
	}
	Serial.print("child count after fill: ");
	Serial.println(lv_obj_get_child_cnt(obj));
	Serial.printf("messageView start index: %d, count: %d\n", messageView.startIndex, messageView.count);
	delay(5);
/*	lv_obj_add_event_cb(lv_obj_get_child(obj, 0), [](lv_event_t* event){
		Serial.println("clicked first");
		if(lv_event_get_key(event) != LV_KEY_PREV) return;
//		static_cast<ConvoBox*>(lv_event_get_user_data(event))->loadAbove();
	}, LV_EVENT_ALL, this);
	lv_obj_add_event_cb(lv_obj_get_child(obj, -1), [](lv_event_t* event){
		Serial.println("clicked last");
		if(lv_event_get_key(event) != LV_KEY_NEXT) return;
//		static_cast<ConvoBox*>(lv_event_get_user_data(event))->loadBelow();
	}, LV_EVENT_ALL, this);*/
}

void ConvoBox::loadAbove(){
	if(convo->messageCount == 0 || messageView.startIndex == 0){
		Serial.println("loaded last focused");
		lv_obj_scroll_to_view(lv_obj_get_child(obj, lastFocused), LV_ANIM_ON);
		lv_group_focus_obj(lv_obj_get_child(obj, lastFocused));
		return;
	}

	int index = (int)messageView.startIndex;
	int newIndex = max(0, (int)messageView.startIndex - 5);
	messageView.update(newIndex, min((uint)bufferLength, convo->messageCount));
	fillMessages();
	Serial.printf("index: %d, newIndex: %d\n", index, newIndex);
	lv_group_focus_obj(lv_obj_get_child(obj, index - newIndex + bufferLength));
}

void ConvoBox::loadBelow(){
	if(convo->messageCount == 0 || messageView.startIndex + bufferLength == convo->messageCount){
		Serial.println("loaded last focused");
		lv_obj_scroll_to_view(lv_obj_get_child(obj, lastFocused), LV_ANIM_ON);
		lv_group_focus_obj(lv_obj_get_child(obj, lastFocused));
		return;
	}

	int index = (int)messageView.startIndex;
	int newIndex = max(0, (int)convo->messageCount - ((int)messageView.startIndex + bufferLength + 5));
	messageView.update(newIndex, min((uint)bufferLength, convo->messageCount));
	fillMessages();
	lv_group_focus_obj(lv_obj_get_child(obj, newIndex - index));
}
