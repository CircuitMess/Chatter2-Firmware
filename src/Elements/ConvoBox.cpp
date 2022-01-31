#include "ConvoBox.h"
#include "../InputLVGL.h"

ConvoBox::ConvoBox(lv_obj_t* parent, UID_t convo, uint16_t hue) : LVObject(parent), LVSelectable(parent), convo(convo), convoView(convo), hue(hue){

	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_size(obj, lv_pct(100), 20);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(obj, 0, 0);

	lv_obj_set_style_pad_gap(obj, 3, 0);
	lv_obj_set_style_pad_all(obj, 1, 0);

	lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_set_style_outline_pad(obj, 1, 0);
	lv_obj_set_style_outline_width(obj, 1, 0);
	lv_obj_set_style_outline_color(obj, lv_color_white(), 0);
	lv_obj_set_style_outline_color(obj, lv_palette_main(LV_PALETTE_RED), LV_STATE_EDITED);
	lv_obj_set_style_outline_opa(obj, LV_OPA_100, 0);

	lv_group_set_wrap(inputGroup, false);

	fillMessages();
	exit();

	lv_obj_add_event_cb(obj, [](lv_event_t* e){
		auto* box = static_cast<ConvoBox*>(e->user_data);
		box->enter();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(obj, [](lv_event_t* e){
		auto* box = static_cast<ConvoBox*>(e->user_data);
		box->exit();
	}, LV_EVENT_READY, this);

	Messages.addReceivedListener(this);
	Messages.addChangedListener(this);
}

ConvoBox::~ConvoBox(){
	Messages.removeReceivedListener(this);
	Messages.removeChangedListener(this);
}

void ConvoBox::fillMessages(){
	lv_group_remove_all_objs(inputGroup);
	for(auto* msgEl : msgElements){
		delete msgEl;
	}
	msgElements.clear();

	for(const auto& msg : convoView.getMessages()){
		createMessage(msg);
	}

	lv_obj_update_layout(obj);
	lv_obj_invalidate(obj);
}

void ConvoBox::enter(){
	if(msgElements.empty()) return;
	lv_group_focus_obj(msgElements.back()->getLvObj());
}

void ConvoBox::exit(){
	if(msgElements.empty()) return;

	if(!convoView.isLatest()){
		convoView.loadLatest();
		fillMessages();
	}

	for(auto& el : msgElements){
		el->clearFocus();
	}

	lv_obj_invalidate(obj);
	lv_obj_scroll_to_y(obj, LV_COORD_MAX, LV_ANIM_ON);
}

void ConvoBox::checkScroll(){
	size_t start = convoView.getStartIndex();
	lv_obj_t* focusedEl = lv_group_get_focused(inputGroup);
	size_t objIndex = lv_obj_get_index(focusedEl);

	const bool nearTop = objIndex <= 0;
	const bool nearBot = objIndex >= msgElements.size() - 1;
	const bool topStop = start == 0;
	const bool botStop = convoView.isLatest(); // start + ConvoView::Count >= messageView.getTotalMessageCount();
	if((!nearTop || topStop) && (!nearBot || botStop)) return;

	int16_t fromTop = lv_obj_get_y(focusedEl) - lv_obj_get_scroll_y(obj);
	UID_t focusedMsgEl = msgElements[objIndex]->getMsg().uid;

	int delta = (int) floor((float) ConvoView::Count / 2.0f);
	convoView.load(nearTop ? max(0, (int) start - delta) : start + delta);
	fillMessages();
	focusedEl = nullptr;

	for(const auto msgEl : msgElements){
		if(msgEl->getMsg().uid == focusedMsgEl){
			focusedEl = msgEl->getLvObj();
			break;
		}
	}
	if(focusedEl == nullptr) return;

	int16_t scroll = lv_obj_get_y(focusedEl) - fromTop;

	lv_obj_scroll_to_y(obj, 0, LV_ANIM_OFF);
	lv_coord_t diff = -scroll + lv_obj_get_scroll_y(obj);
	lv_obj_scroll_by(obj, 0, diff, LV_ANIM_OFF);

	lv_group_focus_obj(focusedEl);
}

void ConvoBox::addMessage(const Message& msg){
	if(convoView.isLatest()){
		createMessage(msg);
		if(!isActive()){
			lv_obj_scroll_to_y(obj, LV_COORD_MAX, LV_ANIM_ON);
		}
	}
}

void ConvoBox::removeMessage(UID_t uid){
	ConvoMessage* msg = nullptr;
	for(const auto msgEl : msgElements){
		if(msgEl->getMsg().uid == uid){
			msg = msgEl;
			break;
		}
	}
	if(msg == nullptr) return;
	delete msg;
}

void ConvoBox::createMessage(const Message& msg){
	ConvoMessage* msgEl = new ConvoMessage(obj, msg, msg.outgoing ? 0 : hue);
	msgElements.push_back(msgEl);

	lv_group_add_obj(inputGroup, msgEl->getLvObj());
	lv_obj_add_flag(msgEl->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	lv_obj_add_event_cb(msgEl->getLvObj(), [](lv_event_t* e){
		ConvoBox* box = static_cast<ConvoBox*>(e->user_data);
		box->checkScroll();
	}, LV_EVENT_FOCUSED, this);

	lv_obj_add_event_cb(msgEl->getLvObj(), [](lv_event_t* e){
		ConvoBox* box = static_cast<ConvoBox*>(e->user_data);
		lv_obj_t* focused = lv_group_get_focused(box->inputGroup);
		uint32_t index = lv_obj_get_index(focused);
		lv_event_send(box->getLvObj(), EV_CONVOBOX_MSG_SELECTED, box->msgElements[index]);
	}, LV_EVENT_CLICKED, this);
}

void ConvoBox::msgReceived(const Message& msg){
	addMessage(msg);
}

void ConvoBox::msgChanged(const Message& msg){
	if(convoView.isLatest()){
		for(auto el : msgElements){
			if(el->getMsg().uid == msg.uid){
				el->setDelivered(true);
				break;
			}
		}
	}
}
