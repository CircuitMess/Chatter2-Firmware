#include "Inbox.h"
#include "UserWithMessage.h"
#include "font.h"

extern std::vector<Profile> friends;

Inbox::Inbox() : LVScreen(){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);
	lv_obj_set_style_bg_img_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_bg_img_src(obj, "S:/bg.bin", 0);

	newConvoLayout = lv_obj_create(obj);
	lv_group_add_obj(inputGroup, newConvoLayout);
	lv_obj_set_width(newConvoLayout, lv_pct(100));
	lv_obj_set_height(newConvoLayout, LV_SIZE_CONTENT);
	lv_obj_set_layout(newConvoLayout, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(newConvoLayout, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(newConvoLayout, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(newConvoLayout, 3, 0);
	lv_obj_set_style_border_width(newConvoLayout, 1, 0);
	lv_obj_set_style_border_color(newConvoLayout, lv_color_white(), 0);
	lv_obj_set_style_border_opa(newConvoLayout, LV_OPA_100, 0);
	lv_obj_set_style_pad_all(newConvoLayout, 2, 0);

	lv_obj_set_style_bg_color(newConvoLayout, lv_color_black(), LV_STATE_FOCUSED | LV_PART_MAIN);
	lv_obj_set_style_bg_opa(newConvoLayout, LV_OPA_90, LV_STATE_FOCUSED | LV_PART_MAIN);
	lv_obj_set_style_border_width(newConvoLayout, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
	lv_obj_set_style_pad_all(newConvoLayout, 1, LV_PART_MAIN | LV_STATE_FOCUSED);

	plusImg = lv_img_create(newConvoLayout);
	lv_img_set_src(plusImg, LV_SYMBOL_PLUS);
	lv_obj_set_style_text_color(plusImg, lv_color_white(), 0);

	newConvoLabel = lv_label_create(newConvoLayout);
	lv_label_set_text(newConvoLabel, "New conversation");
	lv_obj_set_style_text_font(newConvoLabel, &pixelbasic_7, 0);
	lv_obj_set_style_text_color(newConvoLabel, lv_color_white(), 0);

	lv_obj_add_event_cb(newConvoLayout, [](lv_event_t* event){
		static_cast<Inbox*>(event->user_data)->newConvo();
	}, LV_EVENT_CLICKED, this);

	for(const Profile& profile : friends){
		auto user = new UserWithMessage(obj, profile, "Lorem ipsum dolor sit amet consequentur");
		lv_group_add_obj(inputGroup, user->getLvObj());
		lv_obj_add_event_cb(user->getLvObj(), [](lv_event_t* event){
			static_cast<Inbox*>(event->user_data)->openConvo(lv_obj_get_index(lv_event_get_target(event)));
		}, LV_EVENT_CLICKED, this);
		userElements.push_back(user);
	}

	lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
		lv_obj_t* focused = lv_group_get_focused(group);
		lv_obj_scroll_to_view(focused, LV_ANIM_ON);
		lv_obj_invalidate(lv_obj_get_parent(focused));
	});

}

void Inbox::openConvo(uint32_t index){
	Serial.println(index);
	//TODO - staviti otvaranje Convo contexta

}

void Inbox::newConvo(){
	Serial.println("new convo");
	//TODO - staviti otvaranje menija za odabir prijatelja
}
