#include "ProfileScreen.h"
#include "../Elements/ColorBox.h"
#include "../Elements/EditableAvatar.h"
#include <lvgl.h>
#include "../Fonts/font.h"
#include <Pins.hpp>
#include <Input/Input.h>
#include "../Elements/TextEntry.h"
#include "../Storage/Storage.h"
#include "../Modals/ContextMenu.h"
#include "../Services/MessageService.h"
#include <Settings.h>
#include "../FSLVGL.h"

ProfileScreen::ProfileScreen(UID_t uid, bool editable) : LVScreen(), editable(editable), frend(Storage.Friends.get(uid)), profile(frend.profile){
//styles
	lv_style_init(&textStyle);
	lv_style_set_text_color(&textStyle, lv_color_white());
	lv_style_set_text_font(&textStyle, &pixelbasic7);

	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_border_width(obj, 1, 0);
	lv_obj_set_style_border_color(obj, lv_color_hsv_to_rgb(0, 0, 100), 0);

	buildHeader();
	buildBody();
	if(editable){
		buildFooter();
	}else{
		menu = new ContextMenu(this, {{ "Remove friend", 0 }});
		prompt = new Prompt(this, (String("Remove ") + profile.nickname + " from friends?").c_str());

		lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
		lv_group_add_obj(inputGroup, obj);
		lv_group_focus_obj(obj);
		lv_obj_add_event_cb(obj, [](lv_event_t* e){
			auto screen = static_cast<ProfileScreen*>(e->user_data);
			screen->menu->start();
		}, LV_EVENT_CLICKED, this);

		lv_obj_add_event_cb(menu->getLvObj(), [](lv_event_t* e){
			auto screen = static_cast<ProfileScreen*>(e->user_data);
			screen->prompt->start();
		}, LV_EVENT_CLICKED, this);

		lv_obj_add_event_cb(prompt->getLvObj(), [](lv_event_t* e){
			auto screen = static_cast<ProfileScreen*>(e->user_data);
			Messages.deleteFriend(screen->frend.uid);
			screen->pop();
		}, EV_PROMPT_YES, this);

		lv_obj_add_event_cb(prompt->getLvObj(), [](lv_event_t* e){
			auto screen = static_cast<ProfileScreen*>(e->user_data);
			screen->menu->stop();
		}, EV_PROMPT_NO, this);
	}
}

void ProfileScreen::buildHeader(){
	lv_obj_t* header = lv_obj_create(obj);
	lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(header, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(header, 4, 0);
	lv_obj_set_style_bg_opa(header, LV_OPA_0, 0);
	lv_obj_set_width(header, lv_pct(100));
	lv_obj_set_style_pad_all(header, 1, 0);
	lv_obj_set_style_border_width(header, 1, 0);
	lv_obj_set_style_border_color(header, lv_color_white(), 0);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, 0);

	lv_obj_t* label = lv_label_create(header);
	lv_obj_add_style(label, &textStyle, 0);
	lv_label_set_text(label, "Profile");
	name = new TextEntry(header, profile.nickname, 20);
	name->setPlaceholder("Name");
	name->setTextColor(lv_color_hex(0xf4b41b));
	lv_obj_set_style_text_font(name->getLvObj(), &pixelbasic7, 0);
	if(editable){
		lv_group_add_obj(inputGroup, name->getLvObj());
	}
	lv_obj_set_flex_grow(name->getLvObj(), 1);
	lv_obj_set_style_border_width(name->getLvObj(), 1, 0);
	lv_obj_set_style_border_color(name->getLvObj(), lv_color_hsv_to_rgb(0, 0, 100), 0);
	lv_obj_set_style_border_opa(name->getLvObj(), 0, 0);
	lv_obj_set_style_pad_all(name->getLvObj(), 1, 0);
	lv_obj_set_style_border_width(name->getLvObj(), 1, LV_STATE_FOCUSED);
	lv_obj_set_style_border_color(name->getLvObj(), lv_color_hsv_to_rgb(0, 0, 100), LV_STATE_FOCUSED);
	lv_obj_set_style_border_opa(name->getLvObj(), LV_OPA_100, LV_STATE_FOCUSED);
	lv_obj_set_style_bg_opa(name->getLvObj(), LV_OPA_100, LV_STATE_EDITED | LV_PART_MAIN);

	if(editable){
		lv_obj_add_event_cb(name->getLvObj(), [](lv_event_t* event){
			lv_group_focus_freeze((lv_group_t*)(lv_obj_get_group(lv_event_get_target(event))), false);
		}, EV_ENTRY_DONE, nullptr);

		lv_obj_add_event_cb(name->getLvObj(), [](lv_event_t* event){
			TextEntry* textEntry = static_cast<ProfileScreen*>(lv_event_get_user_data(event))->name;
			if(!textEntry->isActive()){
				textEntry->start();
				lv_group_focus_freeze((lv_group_t*)(lv_obj_get_group(textEntry->getLvObj())), true);
			}
		}, LV_EVENT_PRESSED, this);

		lv_obj_add_event_cb(name->getLvObj(), [](lv_event_t* event){
			lv_group_focus_freeze((lv_group_t*)(lv_obj_get_group(lv_event_get_target(event))), false);
		}, EV_ENTRY_CANCEL, nullptr);
	}
	lv_obj_set_height(header, LV_SIZE_CONTENT);

}

void ProfileScreen::buildBody(){
	lv_obj_t* body = lv_obj_create(obj);
	lv_obj_set_layout(body, LV_LAYOUT_FLEX);
	lv_obj_set_style_bg_opa(body, 0, 0);
	lv_obj_set_flex_flow(body, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(body, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(body, 3, 0);
	lv_obj_set_style_pad_ver(body, 5, 0);
	lv_obj_set_style_pad_hor(body, 1, 0);
	lv_obj_set_scrollbar_mode(body, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_width(body, lv_pct(100));
	lv_obj_set_style_border_width(body, 1, 0);
	lv_obj_set_style_border_color(body, lv_color_white(), 0);
	lv_obj_set_style_border_side(body, LV_BORDER_SIDE_BOTTOM, 0);

	if(editable){
		editableAvatar = new EditableAvatar(body, profile.avatar, true);
		lv_group_add_obj(inputGroup, editableAvatar->getLvObj());
	}else{
		avatar = new Avatar(body, profile.avatar, true);
	}

	lv_obj_t* colorObj = lv_obj_create(body);
	lv_obj_set_flex_flow(colorObj, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(colorObj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(colorObj, 3, 0);
	lv_obj_set_style_bg_opa(colorObj, 0, 0);

	lv_obj_t* colorLabel = lv_label_create(colorObj);
	lv_obj_add_style(colorLabel, &textStyle, 0);
	lv_label_set_text(colorLabel, "Color");

	cbox = new ColorBox(colorObj, profile.hue);
	if(editable){
		lv_group_add_obj(inputGroup, cbox->getLvObj());
	}else{
		lv_obj_set_flex_grow(body, 1);
	}
	lv_obj_set_size(colorObj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

	lv_obj_refr_size(colorLabel);
	lv_obj_refr_size(cbox->getLvObj());
	lv_obj_refr_size(colorObj);
	lv_obj_update_layout(colorObj);
	lv_obj_set_width(colorObj, lv_obj_get_width(colorObj) + 5);

	lv_obj_set_height(body, LV_SIZE_CONTENT);
}

void ProfileScreen::buildFooter(){
	lv_obj_t* footer = lv_obj_create(obj);
	lv_obj_set_flex_flow(footer, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(footer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_style_pad_gap(footer, 4, 0);
	lv_obj_set_style_pad_all(footer, 4, 0);
	lv_obj_set_width(footer, lv_pct(100));
	lv_obj_set_style_bg_opa(footer, 0, 0);
	lv_obj_set_flex_grow(footer, 1);

	lv_obj_t* friends = lv_label_create(footer);
	lv_obj_t* received = lv_label_create(footer);
	lv_obj_t* sent = lv_label_create(footer);
	lv_label_set_recolor(friends, true);
	lv_label_set_recolor(received, true);
	lv_label_set_recolor(sent, true);
	lv_obj_add_style(friends, &textStyle, 0);
	lv_obj_add_style(received, &textStyle, 0);
	lv_obj_add_style(sent, &textStyle, 0);

	char buf[50];
	sprintf(buf, "Friends : #f4b41b %lu #", Storage.Friends.all().size() > 1 ? Storage.Friends.all().size() - 1 : 0);
	lv_label_set_text(friends, buf);
	sprintf(buf, "Messages received : #f4b41b %d #", Settings.get().messagesReceived);
	lv_label_set_text(received, buf);
	sprintf(buf, "Messages sent : #f4b41b %d #", Settings.get().messagesSent);
	lv_label_set_text(sent, buf);
	lv_obj_set_height(footer, LV_SIZE_CONTENT);

}

void ProfileScreen::onStart(){
	Input::getInstance()->addListener(this);
	if(!editable){
		Profiles.addListener(this);
	}
}

void ProfileScreen::onStop(){
	Input::getInstance()->removeListener(this);
	if(editable){
		strncpy(profile.nickname, name->getText().c_str(), 21);
		profile.avatar = editableAvatar->getIndex();
		profile.hue = cbox->getHue();
		Profiles.setMyProfile(profile);
	}else{
		Profiles.removeListener(this);
	}
}

void ProfileScreen::buttonPressed(uint i){
	if((!prompt || !prompt->isActive()) && (!menu || !menu->isActive()) && i == BTN_BACK && ((editable && !lv_group_get_editing(inputGroup)) || !editable)){
		pop();
	}
}

void ProfileScreen::profileChanged(const Friend &fren){
	if(editable || fren.uid != frend.uid) return;

	avatar->changeImage(fren.profile.avatar);
	name->setText(fren.profile.nickname);
	cbox->setColor(fren.profile.hue);
	lv_obj_invalidate(obj);
}

ProfileScreen::~ProfileScreen(){
	lv_style_reset(&textStyle);
}
