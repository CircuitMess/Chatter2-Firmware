#include "ConvoBox.h"
#include "../InputLVGL.h"

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

	group = lv_group_create();
	lv_group_set_wrap(group, false);

	fillMessages();

	lv_obj_scroll_to_view(msgElements.back()->getLvObj(), LV_ANIM_OFF);
}

ConvoBox::~ConvoBox(){
	lv_group_del(group);
}

void ConvoBox::fillMessages(){
	lv_group_remove_all_objs(group);
	for(auto* msgEl : msgElements){
		delete msgEl;
	}
	msgElements.clear();

	for(const auto& message : messageView.getMessages()){
		ConvoMessage* msgEl = new ConvoMessage(obj, message, 0);
		msgElements.push_back(msgEl);

		lv_group_add_obj(group, msgEl->getLvObj());
		lv_obj_add_flag(msgEl->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);
		lv_obj_add_event_cb(msgEl->getLvObj(), [](lv_event_t* e){
			ConvoBox* box = static_cast<ConvoBox*>(e->user_data);
			box->checkScroll();
		}, LV_EVENT_FOCUSED, this);
	}

	lv_obj_update_layout(obj);
	lv_obj_invalidate(obj);
}

void ConvoBox::focus(){
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), group);
	lv_group_focus_obj(msgElements.back()->getLvObj());
}

void ConvoBox::defocus(){
	lv_obj_scroll_to_y(obj, LV_COORD_MAX, LV_ANIM_ON);
}

void ConvoBox::checkScroll(){
	size_t start = messageView.getStartIndex();
	lv_obj_t* focusedEl = lv_group_get_focused(group);
	size_t objIndex = lv_obj_get_index(focusedEl);

	const bool nearTop = objIndex <= 0;
	const bool nearBot = objIndex >= MessageView::Count - 1;
	const bool topStop = start == 0;
	const bool botStop = start + MessageView::Count == messageView.getTotalMessageCount();
	if((!nearTop || topStop) && (!nearBot || botStop)) return;

	int16_t fromTop = lv_obj_get_y(focusedEl) - lv_obj_get_scroll_y(obj);
	UID_t focusedMsgEl = msgElements[objIndex]->getMsg().uid;

	messageView.load(nearTop ? max(0, (int) start - 4) : start + 4);
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
