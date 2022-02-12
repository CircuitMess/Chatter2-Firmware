#include "PairScreen.h"
#include "../Elements/User.h"
#include "../Fonts/font.h"
#include "../Elements/Avatar.h"
#include <string>
#include "ProfileScreen.h"

PairScreen::PairScreen() : LVScreen(), autoPop(this){
	lv_obj_set_height(obj, lv_pct(100));
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_gap(obj, 14, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);

	setupScan();

	lv_style_init(&btnDefault);
	lv_style_set_pad_all(&btnDefault, 3);

	lv_style_init(&btnFocused);
	lv_style_set_pad_all(&btnFocused, 2);
	lv_style_set_border_opa(&btnFocused, LV_OPA_100);
	lv_style_set_border_color(&btnFocused, lv_color_white());
	lv_style_set_border_width(&btnFocused, 1);
	lv_style_set_bg_color(&btnFocused, lv_color_make(205, 96, 147));
	lv_style_set_bg_opa(&btnFocused, LV_OPA_100);

	pair.setDoneCallback([](bool success, void* data){
		if(success){
			static_cast<PairScreen*>(data)->pairDone();
		}else{
			static_cast<PairScreen*>(data)->pairFail();
		}
	}, this);
}

PairScreen::~PairScreen(){
	lv_style_reset(&btnDefault);
	lv_style_reset(&btnFocused);
}

void PairScreen::selectUser(uint32_t index){
	autoPop.stop();
	activeIndex = index;
	pair.setUserFoundCallback(nullptr, this);
	pair.setUserChangedCallback(nullptr, this);

	lv_anim_del(scanLabel, threeDotsAnim);
	lv_obj_del(scanLabel);
	if(userCount == 1){
		lv_obj_del(singleUserLayout);
	}else{
		lv_obj_del(userList);
	}
	createPairPrompt(index);
}

void PairScreen::userFound(const Friend &user){
	userCount++;
	if(userCount == 1){
		lv_obj_del(pairAnimation);
		createSingleUser(user);
	}else if(userCount > 1){
		if(userCount == 2){
			lv_obj_del(singleUserLayout);
			createUserList();
			Friend temp;
			temp.profile = pair.getFoundProfiles()[0];
			temp.uid = ESP.getEfuseMac();
			addUserToList(temp);
		}
		addUserToList(user);
	}
}

void PairScreen::createSingleUser(const Friend &user){
	const Profile &prof = user.profile;
	singleUserLayout = lv_obj_create(obj);
	lv_obj_clear_flag(singleUserLayout, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_layout(singleUserLayout, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(singleUserLayout, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(singleUserLayout, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(singleUserLayout, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_bg_color(singleUserLayout, lv_color_make(86, 64, 100), 0);
	lv_obj_set_style_bg_opa(singleUserLayout, LV_OPA_100, 0);

	lv_obj_t* avatarContainer = lv_obj_create(singleUserLayout);
	lv_obj_clear_flag(avatarContainer, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_size(avatarContainer, lv_pct(100), 60);
	lv_obj_set_style_pad_ver(avatarContainer, 8, 0);
	lv_obj_set_style_border_width(avatarContainer, 1, 0);
	lv_obj_set_style_border_color(avatarContainer, lv_color_white(), 0);
	lv_obj_set_style_border_opa(avatarContainer, LV_OPA_100, 0);
	lv_obj_set_style_border_side(avatarContainer, LV_BORDER_SIDE_FULL & ~LV_BORDER_SIDE_BOTTOM, 0);

	auto avatar = new Avatar(avatarContainer, prof.avatar, true);
	lv_obj_align(avatar->getLvObj(), LV_ALIGN_CENTER, 0, 0);

	lv_obj_t* labelContainer = lv_obj_create(singleUserLayout);
	lv_obj_clear_flag(labelContainer, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_size(labelContainer, lv_pct(100), LV_SIZE_CONTENT);
	lv_obj_set_style_pad_all(labelContainer, 5, 0);
	lv_obj_set_style_border_width(labelContainer, 1, 0);
	lv_obj_set_style_border_color(labelContainer, lv_color_white(), 0);
	lv_obj_set_style_border_opa(labelContainer, LV_OPA_100, 0);

	lv_obj_t* label = lv_label_create(labelContainer);
	lv_label_set_text(label, prof.nickname);
	lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_text_font(label, &pixelbasic16, 0);
	lv_obj_set_style_text_color(label, lv_color_white(), 0);

	lv_group_add_obj(inputGroup, singleUserLayout);
	lv_obj_add_event_cb(singleUserLayout, [](lv_event_t* event){
		static_cast<PairScreen*>(event->user_data)->selectUser(0);
	}, LV_EVENT_PRESSED, this);

	lv_obj_set_height(singleUserLayout, LV_SIZE_CONTENT);
}

void PairScreen::createUserList(){
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

void PairScreen::addUserToList(const Friend &user){
	auto newUser = new User(userList, user);
	lv_group_add_obj(inputGroup, newUser->getLvObj());
	lv_obj_add_event_cb(newUser->getLvObj(), [](lv_event_t* event){
		static_cast<PairScreen*>(event->user_data)->selectUser(lv_obj_get_index(lv_event_get_target(event)));
	}, LV_EVENT_PRESSED, this);
	lv_obj_add_flag(newUser->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);
}

void PairScreen::onStart(){
	pair.begin();

	autoPop.start();
}

void PairScreen::onStop(){
	autoPop.stop();
}

void PairScreen::userChanged(const Friend &user, uint32_t index){
	if(userCount > 1){
		lv_obj_del(lv_obj_get_child(userList, index));

		auto newUser = new User(userList, user);
		lv_group_add_obj(inputGroup, newUser->getLvObj());
		lv_obj_add_event_cb(newUser->getLvObj(), [](lv_event_t* event){
			static_cast<PairScreen*>(event->user_data)->selectUser(lv_obj_get_index(lv_event_get_target(event)));
		}, LV_EVENT_PRESSED, this);
		lv_obj_add_flag(newUser->getLvObj(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);
		lv_obj_move_to_index(newUser->getLvObj(), index);
		lv_obj_invalidate(userList);
	}else if(userCount == 1){
		lv_obj_del(singleUserLayout);
		createSingleUser(user);
	}
	lv_obj_invalidate(obj);
}

void PairScreen::pairDone(){
	lv_obj_del(waitLayout);
	lv_obj_del(scanLabel);
	lv_anim_del(scanLabel, threeDotsAnim);

	lv_obj_t* doneLayout = lv_obj_create(obj);
	lv_obj_clear_flag(doneLayout, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_height(doneLayout, 120);
	lv_obj_t* doneLabel = lv_label_create(doneLayout);
	lv_obj_align(doneLabel, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(doneLabel, "Pairing successful!");
	lv_obj_set_style_text_font(doneLabel, &pixelbasic16, 0);
	lv_obj_set_style_text_color(doneLabel, lv_color_white(), 0);

	lv_obj_t* pressLabel = lv_label_create(doneLayout);
	lv_obj_align(pressLabel, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_label_set_text(pressLabel, "Press ENTER to continue.");
	lv_obj_set_style_text_font(pressLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(pressLabel, lv_color_white(), 0);

	lv_group_add_obj(inputGroup, doneLayout);
	lv_obj_add_event_cb(doneLayout, [](lv_event_t* event){
		lv_group_set_editing((lv_group_t*)lv_obj_get_group(lv_event_get_target(event)), false);
//		lv_obj_del(lv_event_get_target(event));

		auto pairScreen = static_cast<PairScreen*>(event->user_data);
		pairScreen->pop();
	}, LV_EVENT_KEY, this);

	lv_obj_add_event_cb(doneLayout, [](lv_event_t* event){
		lv_group_set_editing((lv_group_t*)lv_obj_get_group(lv_event_get_target(event)), false);
//		lv_obj_del(lv_event_get_target(event));

		auto pairScreen = static_cast<PairScreen*>(event->user_data);
		pairScreen->pop();
	}, LV_EVENT_PRESSED, this);

	lv_group_set_editing(inputGroup, true);
}

void PairScreen::pairFail(){
	lv_obj_del(waitLayout);
	lv_obj_del(scanLabel);
	lv_anim_del(scanLabel, threeDotsAnim);

	lv_obj_t* doneLayout = lv_obj_create(obj);
	lv_obj_clear_flag(doneLayout, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_size(doneLayout, lv_pct(100), 120);
	lv_obj_t* doneLabel = lv_label_create(doneLayout);
	lv_obj_align(doneLabel, LV_ALIGN_CENTER, 0, -20);
	lv_label_set_text(doneLabel, "Pairing failed!");
	lv_obj_set_style_text_font(doneLabel, &pixelbasic16, 0);
	lv_obj_set_style_text_color(doneLabel, lv_color_white(), 0);

	lv_obj_t* pressLabel = lv_label_create(doneLayout);
	lv_obj_align(pressLabel, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_label_set_long_mode(pressLabel, LV_LABEL_LONG_WRAP);
	lv_label_set_text(pressLabel,
					  "Try bringing the Chatters closer together and make sure there's a clear line of sight between them.\n\nPress ENTER to continue.");
	lv_obj_set_style_text_align(pressLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_style_text_font(pressLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(pressLabel, lv_color_white(), 0);
	lv_obj_set_size(pressLabel, lv_pct(100), LV_SIZE_CONTENT);
	lv_obj_set_width(doneLayout, lv_pct(100));

	lv_group_add_obj(inputGroup, doneLayout);
	lv_obj_add_event_cb(doneLayout, [](lv_event_t* event){
		lv_group_set_editing((lv_group_t*)lv_obj_get_group(lv_event_get_target(event)), false);

		lv_obj_del(lv_event_get_target(event));
		static_cast<PairScreen*>(event->user_data)->setupScan();
	}, LV_EVENT_KEY, this);

	lv_obj_add_event_cb(doneLayout, [](lv_event_t* event){
		lv_group_set_editing((lv_group_t*)lv_obj_get_group(lv_event_get_target(event)), false);
		lv_obj_del(lv_event_get_target(event));
		static_cast<PairScreen*>(event->user_data)->setupScan();
	}, LV_EVENT_PRESSED, this);

	lv_group_set_editing(inputGroup, true);
}

void PairScreen::createPairPrompt(uint32_t index){
	const Profile &prof = pair.getFoundProfiles()[index];
	pairPrompt = lv_obj_create(obj);
	lv_obj_set_style_pad_ver(pairPrompt, 15, 0);
	lv_obj_clear_flag(pairPrompt, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_layout(pairPrompt, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(pairPrompt, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(pairPrompt, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(pairPrompt, LV_SCROLLBAR_MODE_OFF);

	lv_obj_t* avatarContainer = lv_obj_create(pairPrompt);
	lv_obj_clear_flag(avatarContainer, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_size(avatarContainer, lv_pct(100), 70);
	lv_obj_set_style_pad_ver(avatarContainer, 3, 0);
	lv_obj_set_style_pad_hor(avatarContainer, 3, 0);
	lv_obj_set_style_border_width(avatarContainer, 1, 0);
	lv_obj_set_style_border_color(avatarContainer, lv_color_white(), 0);
	lv_obj_set_style_border_opa(avatarContainer, LV_OPA_100, 0);
	lv_obj_set_style_border_side(avatarContainer, LV_BORDER_SIDE_FULL & ~LV_BORDER_SIDE_BOTTOM, 0);

	lv_obj_set_style_bg_color(avatarContainer, lv_color_make(86, 64, 100), 0);
	lv_obj_set_style_bg_opa(avatarContainer, LV_OPA_100, 0);

	auto avatar = new Avatar(avatarContainer, prof.avatar, true);
	lv_obj_align(avatar->getLvObj(), LV_ALIGN_TOP_MID, 0, 0);

	char temp[30];
	sprintf(temp, "Add %s?", prof.nickname);
	lv_obj_t* addLabel = lv_label_create(avatarContainer);
	lv_label_set_text(addLabel, temp);
	lv_label_set_long_mode(addLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
	lv_obj_align(addLabel, LV_ALIGN_BOTTOM_LEFT, 0, 0);
	lv_obj_set_style_text_font(addLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(addLabel, lv_color_white(), 0);

	lv_obj_t* labelContainer = lv_obj_create(pairPrompt);
	lv_obj_clear_flag(labelContainer, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_size(labelContainer, lv_pct(100), LV_SIZE_CONTENT);
	lv_obj_set_style_border_width(labelContainer, 1, 0);
	lv_obj_set_style_border_color(labelContainer, lv_color_white(), 0);
	lv_obj_set_style_border_opa(labelContainer, LV_OPA_100, 0);
	lv_obj_set_style_bg_color(labelContainer, lv_color_make(86, 64, 100), 0);
	lv_obj_set_style_bg_opa(labelContainer, LV_OPA_100, 0);

	lv_obj_t* yesBtn = lv_btn_create(labelContainer);
	lv_obj_align(yesBtn, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_size(yesBtn, lv_pct(50), LV_SIZE_CONTENT);
	lv_obj_t* yesLabel = lv_label_create(yesBtn);
	lv_obj_set_style_text_font(yesLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(yesLabel, lv_color_white(), 0);
	lv_obj_align(yesLabel, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(yesLabel, "YES");
	lv_obj_set_height(yesBtn, LV_SIZE_CONTENT);

	lv_obj_t* noBtn = lv_btn_create(labelContainer);
	lv_obj_align(noBtn, LV_ALIGN_RIGHT_MID, 0, 0);
	lv_obj_set_size(noBtn, lv_pct(50), LV_SIZE_CONTENT);
	lv_obj_t* noLabel = lv_label_create(noBtn);
	lv_obj_set_style_text_font(noLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(noLabel, lv_color_white(), 0);
	lv_obj_align(noLabel, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(noLabel, "NO");
	lv_obj_set_height(noBtn, LV_SIZE_CONTENT);

	lv_obj_set_height(labelContainer, LV_SIZE_CONTENT);

	lv_obj_add_style(yesBtn, &btnDefault, LV_STATE_DEFAULT);
	lv_obj_add_style(noBtn, &btnDefault, LV_STATE_DEFAULT);
	lv_obj_add_style(yesBtn, &btnFocused, LV_STATE_FOCUSED);
	lv_obj_add_style(noBtn, &btnFocused, LV_STATE_FOCUSED);
	lv_group_add_obj(inputGroup, yesBtn);
	lv_group_add_obj(inputGroup, noBtn);

	lv_obj_set_height(pairPrompt, LV_SIZE_CONTENT);

	lv_obj_add_event_cb(yesBtn, [](lv_event_t* event){
		static_cast<PairScreen*>(lv_event_get_user_data(event))->confirmPair();
	}, LV_EVENT_PRESSED, this);

	lv_obj_add_event_cb(noBtn, [](lv_event_t* event){
		static_cast<PairScreen*>(lv_event_get_user_data(event))->refusePair();
	}, LV_EVENT_PRESSED, this);

	lv_obj_add_event_cb(yesBtn, [](lv_event_t* event){
		if(lv_event_get_key(event) == LV_KEY_ESC){
			static_cast<PairScreen*>(lv_event_get_user_data(event))->refusePair();
		}
	}, LV_EVENT_KEY, this);

	lv_obj_add_event_cb(noBtn, [](lv_event_t* event){
		if(lv_event_get_key(event) == LV_KEY_ESC){
			static_cast<PairScreen*>(lv_event_get_user_data(event))->refusePair();
		}
	}, LV_EVENT_KEY, this);
}

void PairScreen::createWaiting(){
	waitLayout = lv_obj_create(obj);
	lv_obj_clear_flag(waitLayout, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_scrollbar_mode(waitLayout, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_size(waitLayout, lv_pct(100), 100);
	lv_obj_t* waitSpinner = lv_spinner_create(waitLayout, 1000, 60);
	lv_obj_align(waitSpinner, LV_ALIGN_TOP_MID, 0, 0);
	lv_obj_set_size(waitSpinner, 80, 80);
	lv_obj_set_style_pad_all(waitSpinner, 10, 0);
	lv_obj_set_style_arc_color(waitSpinner, lv_color_white(), 0);
	lv_obj_set_style_arc_color(waitSpinner, lv_color_white(), LV_PART_INDICATOR);
	lv_obj_set_style_arc_opa(waitSpinner, LV_OPA_50, 0);
	lv_obj_set_style_arc_opa(waitSpinner, LV_OPA_100, LV_PART_INDICATOR);
	lv_obj_set_style_arc_width(waitSpinner, 5, 0);
	lv_obj_set_style_arc_width(waitSpinner, 5, LV_PART_INDICATOR);

	lv_group_add_obj(inputGroup, waitLayout);
	lv_obj_add_event_cb(waitLayout, [](lv_event_t* event){
		if(lv_event_get_key(event) == LV_KEY_ESC){
			static_cast<PairScreen*>(lv_event_get_user_data(event))->cancelPair();
		}
	}, LV_EVENT_KEY, this);

	lv_obj_t* cancelLabel = lv_label_create(waitLayout);
	lv_obj_align(cancelLabel, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_label_set_text(cancelLabel, "Press BACK to cancel pairing.");
	lv_obj_set_style_text_font(cancelLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(cancelLabel, lv_color_white(), 0);
	lv_obj_set_style_pad_all(cancelLabel, 5, 0);
	lv_obj_set_width(cancelLabel, lv_pct(100));

	pair.setResponseCallback([](void* data){
		auto screen = static_cast<PairScreen*>(data);
		screen->pair.setResponseCallback(nullptr, nullptr);
		lv_label_set_text(screen->scanLabel, "Pairing");
		lv_anim_del(screen->scanLabel, PairScreen::threeDotsAnim);
		lv_anim_start(&screen->scanningAnim);
		lv_obj_add_flag(lv_obj_get_child(screen->waitLayout, 1), LV_OBJ_FLAG_HIDDEN);
		lv_obj_remove_event_cb(screen->waitLayout, nullptr);
	}, this);
}

void PairScreen::threeDotsAnim(void* obj, int32_t v){
	if(v == 4) return;

	auto* label = (lv_obj_t*)obj;
	std::string temp = lv_label_get_text(label);
	temp = temp.substr(0, temp.find('.'));

	for(int i = 0; i < v; ++i){
		temp.append(".");
	}
	lv_label_set_text((lv_obj_t*)obj, temp.c_str());
}

void PairScreen::createScanLabel(){
	scanLabel = lv_label_create(obj);
	lv_label_set_text(scanLabel, "Scanning...");
	lv_obj_set_style_text_font(scanLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(scanLabel, lv_color_white(), 0);
	lv_obj_set_style_border_width(scanLabel, 1, 0);
	lv_obj_set_style_border_color(scanLabel, lv_color_white(), 0);
	lv_obj_set_style_border_opa(scanLabel, LV_OPA_100, 0);
	lv_obj_set_style_pad_all(scanLabel, 2, 0);
	lv_obj_set_width(scanLabel, lv_pct(100));
}

void PairScreen::confirmPair(){
	pair.requestPair(activeIndex);
	lv_obj_del(pairPrompt);
	createScanLabel();
	lv_label_set_text(scanLabel, "Waiting for friend to pair");
	lv_anim_set_exec_cb(&scanningAnim, threeDotsAnim);
	lv_anim_set_var(&scanningAnim, scanLabel);
	lv_anim_start(&scanningAnim);
	createWaiting();
}

void PairScreen::refusePair(){
	lv_obj_del(pairPrompt);
	lv_anim_del(scanLabel, threeDotsAnim);
	uint32_t temp = userCount;

	setupScan();

	for(auto &prof: pair.getFoundProfiles()){
		Friend fren;
		fren.profile = prof;
		fren.uid = ESP.getEfuseMac(); //to prevent ProfileService from changing the User element
		userFound(fren);
	}

}

void PairScreen::setupScan(){
	autoPop.start();
	userCount = 0;
	createScanLabel();
	lv_anim_init(&scanningAnim);
	lv_anim_set_var(&scanningAnim, scanLabel);
	lv_anim_set_time(&scanningAnim, 4000);
	lv_anim_set_repeat_count(&scanningAnim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_path_cb(&scanningAnim, lv_anim_path_linear);
	lv_anim_set_exec_cb(&scanningAnim, threeDotsAnim);
	lv_anim_set_values(&scanningAnim, 0, 4);
	lv_anim_start(&scanningAnim);

	pairAnimation = lv_gif_create(obj);
	lv_gif_set_src(pairAnimation, "S:/pairing.gif");
	lv_obj_set_size(pairAnimation, 42, 77);

	pair.setUserFoundCallback([](const Profile &prof, void* data){
		Friend fren;
		fren.profile = prof;
		fren.uid = ESP.getEfuseMac(); //to prevent ProfileService from changing the User element
		//not setting encKey (not used anywhere inside User element)
		static_cast<PairScreen*>(data)->userFound(fren);
	}, this);

	pair.setUserChangedCallback([](const Profile &prof, int index, void* data){
		Friend fren;
		fren.profile = prof;
		fren.uid = ESP.getEfuseMac(); //to prevent ProfileService from changing the User element
		//not setting encKey (not used anywhere inside User element)
		static_cast<PairScreen*>(data)->userChanged(fren, index);
	}, this);
}

void PairScreen::cancelPair(){
	pair.cancelPair();

	lv_obj_del(waitLayout);
	lv_anim_del(scanLabel, threeDotsAnim);
	lv_obj_del(scanLabel);

	setupScan();

	for(auto &prof: pair.getFoundProfiles()){
		Friend fren;
		fren.profile = prof;
		fren.uid = ESP.getEfuseMac(); //to prevent ProfileService from changing the User element
		userFound(fren);
	}

}

