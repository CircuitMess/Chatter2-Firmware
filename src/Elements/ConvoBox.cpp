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
	lv_obj_set_style_pad_hor(obj, 1, 0);
	lv_obj_set_style_pad_ver(obj, 2, 0);

	lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

/*	lv_obj_set_style_outline_pad(obj, 1, 0);
	lv_obj_set_style_outline_width(obj, 1, 0);
	lv_obj_set_style_outline_color(obj, lv_color_white(), 0);
	lv_obj_set_style_outline_color(obj, lv_palette_main(LV_PALETTE_RED), LV_STATE_EDITED);
	lv_obj_set_style_outline_opa(obj, LV_OPA_100, 0);*/

	lv_group_set_wrap(inputGroup, false);

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
	Profiles.addListener(this);
}

ConvoBox::~ConvoBox(){
	Messages.removeReceivedListener(this);
	Messages.removeChangedListener(this);
	Profiles.removeListener(this);
	stopAnim();
}

void ConvoBox::load(){
	convoView.loadLatest();
	lv_obj_scroll_by(obj, 0, lv_obj_get_height(obj), LV_ANIM_OFF);
	fillMessages();
	exit();
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

	if(msgElements.size() == 1){
		lv_obj_scroll_to_y(obj, 0, LV_ANIM_ON);
	}

	lv_obj_update_layout(obj);
	lv_obj_invalidate(obj);
}

void ConvoBox::startAnim(){
	stopAnim();

	lv_obj_t* focused = lv_group_get_focused(inputGroup);
	if(focused == nullptr) return;

	lv_anim_init(&selectedAnim);
	lv_anim_set_var(&selectedAnim, focused);
	lv_anim_set_values(&selectedAnim, -200, 200);
	lv_anim_set_repeat_count(&selectedAnim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_time(&selectedAnim, 600);
	lv_anim_set_playback_time(&selectedAnim, 600);
	lv_anim_set_path_cb(&selectedAnim, lv_anim_path_ease_in_out);
	lv_anim_set_exec_cb(&selectedAnim, [](void* var, int32_t value){
		lv_obj_t* msg = static_cast<lv_obj_t*>(var);
		lv_obj_set_style_translate_x(msg, round((float) value / 100.0f), LV_STATE_DEFAULT | LV_STATE_FOCUSED | LV_PART_MAIN);
	});
	lv_anim_start(&selectedAnim);
}

void ConvoBox::stopAnim(){
	for(auto el : msgElements){
		lv_anim_del(el->getLvObj(), nullptr);
	}
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
	lv_obj_scroll_to_view(lv_obj_get_child(obj, -1), LV_ANIM_ON);
	stopAnim();
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
			lv_obj_scroll_to_view(lv_obj_get_child(obj, -1), LV_ANIM_ON);
		}
	}

	if(msgElements.size() == 1){
		lv_obj_scroll_to_y(obj, 0, LV_ANIM_ON);
	}

	exit();
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

	auto it = std::find(msgElements.begin(), msgElements.end(), msg);
	if(it == msgElements.end()){
		printf("Msg el not found in vector\n");
		return;
	}

	msgElements.erase(it);
	delete msg;

	exit();
}

void ConvoBox::createMessage(const Message& msg){
	ConvoMessage* msgEl = new ConvoMessage(obj, msg, msg.outgoing ? Profiles.getMyProfile().hue : hue);
	msgElements.push_back(msgEl);

	lv_group_add_obj(inputGroup, msgEl->getLvObj());
	lv_obj_add_flag(msgEl->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	lv_obj_add_event_cb(msgEl->getLvObj(), [](lv_event_t* e){
		ConvoBox* box = static_cast<ConvoBox*>(e->user_data);
		box->checkScroll();
		box->startAnim();
	}, LV_EVENT_FOCUSED, this);

	lv_obj_add_event_cb(msgEl->getLvObj(), [](lv_event_t* e){
		ConvoBox* box = static_cast<ConvoBox*>(e->user_data);
		box->stopAnim();
		lv_obj_t* obj = e->current_target;
		lv_obj_set_style_translate_x(obj, 0, LV_STATE_DEFAULT | LV_STATE_FOCUSED | LV_PART_MAIN);
	}, LV_EVENT_DEFOCUSED, this);

	lv_obj_add_event_cb(msgEl->getLvObj(), [](lv_event_t* e){
		ConvoBox* box = static_cast<ConvoBox*>(e->user_data);
		lv_obj_t* focused = lv_group_get_focused(box->inputGroup);
		uint32_t index = lv_obj_get_index(focused);
		lv_event_send(box->getLvObj(), EV_CONVOBOX_MSG_SELECTED, box->msgElements[index]);
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(msgEl->getLvObj(), [](lv_event_t* e){
		ConvoBox* box = static_cast<ConvoBox*>(e->user_data);
		box->deselect();
		lv_event_send(box->getLvObj(), EV_CONVOBOX_CANCEL, nullptr);
	}, LV_EVENT_CANCEL, this);
}

void ConvoBox::msgReceived(const Message& msg){
	addMessage(msg);
	Messages.markRead(convo);
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

void ConvoBox::profileChanged(const Friend &fren){
	if(fren.uid != convo) return;

	hue = fren.profile.hue;
	for(const auto msgEl : msgElements){
		if(!msgEl->getMsg().outgoing){
			msgEl->setHue(hue);
		}
	}
	lv_obj_invalidate(obj);
}
