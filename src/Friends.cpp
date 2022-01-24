#include "Friends.h"
#include "Model/Profile.hpp"
#include "font.h"

extern std::vector<Profile> friends;

Friends::Friends() : LVScreen(){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);
	lv_obj_set_style_bg_img_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_bg_img_src(obj, "S:/bg.bin", 0);

	lv_obj_t* addLayout = lv_obj_create(obj);
	lv_obj_set_height(addLayout, LV_SIZE_CONTENT);
	lv_obj_set_layout(addLayout,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(addLayout, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(addLayout, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(addLayout, 3, 0);
	lv_obj_set_style_border_width(addLayout, 1, 0);
	lv_obj_set_style_border_color(addLayout, lv_color_white(), 0);
	lv_obj_set_style_border_opa(addLayout, LV_OPA_100, 0);
	lv_obj_set_style_pad_all(addLayout, 2, 0);
	lv_obj_set_width(addLayout, lv_pct(100));
	lv_obj_set_style_border_width(addLayout, 2, LV_STATE_FOCUSED | LV_PART_MAIN);
	lv_obj_set_style_pad_all(addLayout, 1, LV_STATE_FOCUSED | LV_PART_MAIN);
	lv_obj_set_style_bg_color(addLayout, lv_color_black(), LV_STATE_FOCUSED | LV_PART_MAIN);
	lv_obj_set_style_bg_opa(addLayout, LV_OPA_90, LV_STATE_FOCUSED | LV_PART_MAIN);

	lv_obj_t* plusImg = lv_img_create(addLayout);
	lv_img_set_src(plusImg, LV_SYMBOL_PLUS);
	lv_obj_set_style_text_color(plusImg, lv_color_white(), 0);

	lv_obj_t* addFriendLabel = lv_label_create(addLayout);
	lv_label_set_text(addFriendLabel,  "Add friend");
	lv_obj_set_style_text_font(addFriendLabel, &pixelbasic_7, 0);
	lv_obj_set_style_text_color(addFriendLabel, lv_color_white(), 0);

	lv_group_add_obj(inputGroup, addLayout);
	for(const Profile& profile : friends){
		lv_group_add_obj(inputGroup, (new User(obj, profile))->getLvObj());
	}

	lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
		lv_obj_t* focused = lv_group_get_focused(group);
		lv_obj_scroll_to_view(focused, LV_ANIM_ON);
	});
}