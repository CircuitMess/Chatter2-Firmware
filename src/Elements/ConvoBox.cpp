#include "ConvoBox.h"
#include "../ConvoMessage.h"

ConvoBox::ConvoBox(lv_obj_t* parent, UID_t convo) : LVObject(parent), convo(convo), messageView(convo){

	lv_obj_set_size(obj, lv_pct(100), 20);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_bg_color(obj, lv_color_make(86, 64, 100), 0);

	lv_obj_set_style_pad_gap(obj, 3, 0);
	lv_obj_set_style_pad_all(obj, 1, 0);

	lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_set_style_outline_pad(obj, 1, 0);
	lv_obj_set_style_outline_width(obj, 1, 0);
	lv_obj_set_style_outline_color(obj, lv_color_white(), 0);
	lv_obj_set_style_outline_color(obj, lv_palette_main(LV_PALETTE_RED), LV_STATE_EDITED);
	lv_obj_set_style_outline_opa(obj, LV_OPA_100, 0);

/*	lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
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
	}*/

	fillMessages();
}

void ConvoBox::fillMessages(){
	for(auto* msgEl : msgElements){
		delete msgEl;
	}
	msgElements.clear();

	for(const auto& message : messageView.getMessages()){
		new ConvoMessage(obj, message.getText(), message.outgoing, 0, message.received);
	}
}