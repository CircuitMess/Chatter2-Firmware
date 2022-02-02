#include "AddFriend.h"
#include "User.h"

extern std::vector <ProfileStruct> scannedFriends;

AddFriend::AddFriend() : LVScreen(){

	lv_obj_set_height(obj, lv_pct(100));
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_AUTO);
	lv_obj_set_style_pad_gap(obj, 13, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);
	lv_obj_set_style_bg_img_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_bg_img_src(obj, "S:/bg.bin", 0);

	lv_obj_t* scanLabel = lv_label_create(obj);
	lv_label_set_text(scanLabel, "Scanning...");
	lv_obj_set_style_text_font(scanLabel, &pixelbasic_7, 0);
	lv_obj_set_style_text_color(scanLabel, lv_color_white(), 0);
	lv_obj_set_style_border_width(scanLabel, 1, 0);
	lv_obj_set_style_border_color(scanLabel, lv_color_white(), 0);
	lv_obj_set_style_border_opa(scanLabel, LV_OPA_100, 0);
	lv_obj_set_style_pad_all(scanLabel, 2, 0);
	lv_obj_set_width(scanLabel, lv_pct(100));

	pairAnimation = lv_gif_create(obj);
	lv_gif_set_src(pairAnimation, "S:/pairing.gif");
	lv_obj_set_size(pairAnimation, 42, 77);

	lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
		lv_obj_t* focused = lv_group_get_focused(group);
		lv_obj_scroll_to_view(focused, LV_ANIM_ON);
		lv_obj_invalidate(lv_obj_get_parent(focused));
		lv_obj_invalidate(lv_obj_get_parent(lv_obj_get_parent(focused)));
		lv_obj_invalidate(focused);
	});
}

void AddFriend::selectUser(uint32_t index){
	Serial.println(index);
	//TODO - open add friend menu
}

void AddFriend::userFound(ProfileStruct user){
	scannedFriends.push_back(user);
	if(scannedFriends.size() == 1){
		lv_obj_del(pairAnimation);
		createSingleUser();
		lv_obj_invalidate(obj);
	}else if(scannedFriends.size() > 1){
		if(scannedFriends.size() == 2){
			lv_obj_del(singleUserLayout);
			createUserList();
			addUserToList(scannedFriends[0]);
		}
		addUserToList(user);
	}
}

void AddFriend::createSingleUser(){
	ProfileStruct &prof = scannedFriends.back();
	singleUserLayout = lv_obj_create(obj);
	lv_obj_set_height(singleUserLayout, 84);
	lv_obj_set_layout(singleUserLayout, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(singleUserLayout, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(singleUserLayout, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(singleUserLayout, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_bg_color(singleUserLayout, lv_color_make(86, 64, 100), 0);
	lv_obj_set_style_bg_opa(singleUserLayout, LV_OPA_100, 0);

	lv_obj_t* avatarContainer = lv_obj_create(singleUserLayout);
	lv_obj_set_size(avatarContainer, lv_pct(100), 60);
	lv_obj_set_style_pad_ver(avatarContainer, 8, 0);
	lv_obj_set_style_border_width(avatarContainer, 1, 0);
	lv_obj_set_style_border_color(avatarContainer, lv_color_white(), 0);
	lv_obj_set_style_border_opa(avatarContainer, LV_OPA_100, 0);
	lv_obj_set_style_border_side(avatarContainer, LV_BORDER_SIDE_FULL & ~LV_BORDER_SIDE_BOTTOM, 0);

	lv_obj_t* avatar = lv_obj_create(avatarContainer);
	lv_obj_set_size(avatar, 40, 40);
	lv_obj_set_style_bg_opa(avatar, LV_OPA_100, 0);
	lv_obj_set_style_bg_color(avatar, lv_color_hsv_to_rgb(50, 100, 100), 0);
	lv_obj_set_style_radius(avatar, LV_RADIUS_CIRCLE, 0);
	lv_obj_align(avatar, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t* labelContainer = lv_obj_create(singleUserLayout);
	lv_obj_set_size(labelContainer, lv_pct(100), LV_SIZE_CONTENT);
	lv_obj_set_style_pad_all(labelContainer, 6, 0);
	lv_obj_set_style_border_width(labelContainer, 1, 0);
	lv_obj_set_style_border_color(labelContainer, lv_color_white(), 0);
	lv_obj_set_style_border_opa(labelContainer, LV_OPA_100, 0);

	lv_obj_t* label = lv_label_create(labelContainer);
	lv_label_set_text(label, prof.nickname);
	lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void AddFriend::createUserList(){
	userList = lv_obj_create(obj);
	lv_obj_set_width(userList, lv_pct(100));
	lv_obj_set_flex_grow(userList, 1);
	lv_obj_set_size(userList, lv_pct(100), lv_pct(100));
	lv_obj_set_layout(userList, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(userList, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(userList, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(userList, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_scrollbar_mode(userList, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_add_flag(userList, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_set_style_pad_gap(obj, 5, 0);
}

void AddFriend::addUserToList(ProfileStruct &user){
	auto newUser = new User(userList, user);
	lv_group_add_obj(inputGroup, newUser->getLvObj());
	lv_obj_add_event_cb(newUser->getLvObj(), [](lv_event_t* event){
		static_cast<AddFriend*>(event->user_data)->selectUser(lv_obj_get_index(lv_event_get_target(event)));
	}, LV_EVENT_CLICKED, this);

	lv_obj_invalidate(obj);
	lv_obj_invalidate(userList);
}

