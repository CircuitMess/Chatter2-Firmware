#include "ProfileScreen.h"
#include "ColorBox.h"
#include "EditableAvatar.h"
#include <lvgl.h>
#include "font.h"
#include <Pins.hpp>
#include <Input/Input.h>
#include "TextEntry.h"

ProfileScreen::ProfileScreen(const Profile& profile, bool editable) : LVScreen(), editable(editable), profile(profile){
//styles
	lv_style_init(&textStyle);
	lv_style_set_text_color(&textStyle, lv_color_white());
	lv_style_set_bg_opa(&textStyle, 0);
	lv_style_set_text_font(&textStyle, &pixelbasic_7);

	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_bg_img_src(obj, "S:/bg.bin", 0);
	lv_obj_set_style_border_width(obj, 1, 0);
	lv_obj_set_style_border_color(obj, lv_color_hsv_to_rgb(0, 0, 100), 0);

	buildHeader();
	buildBody();
	buildFooter();
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

	name = new TextEntry(header, profile.nickname, 11);
	name->setPlaceholder("Name");
	name->setTextColor(lv_color_hsv_to_rgb(21, 91, 100));
	if(editable){
		lv_group_add_obj(inputGroup, name->getLvObj());
	}
	lv_obj_set_style_bg_opa(name->getLvObj(), LV_OPA_0, 0);
	lv_obj_set_style_bg_color(name->getLvObj(), lv_color_white(), 0);
	lv_obj_set_style_text_font(name->getLvObj(), &pixelnickname24, 0);
	lv_obj_set_style_opa(name->getLvObj(), LV_OPA_0, LV_PART_CURSOR | LV_STATE_FOCUSED);
	lv_obj_set_flex_grow(name->getLvObj(), 1);
	lv_obj_set_style_border_width(name->getLvObj(), 1, 0);
	lv_obj_set_style_border_color(name->getLvObj(), lv_color_hsv_to_rgb(0, 0, 100), 0);
	lv_obj_set_style_border_opa(name->getLvObj(), 0, 0);
	lv_obj_set_style_pad_all(name->getLvObj(), 1, 0);
	lv_obj_set_style_border_width(name->getLvObj(), 1, LV_STATE_FOCUSED);
	lv_obj_set_style_border_color(name->getLvObj(), lv_color_hsv_to_rgb(0, 0, 100), LV_STATE_FOCUSED);
	lv_obj_set_style_border_opa(name->getLvObj(), LV_OPA_100, LV_STATE_FOCUSED);

	lv_obj_add_event_cb(name->getLvObj(), [](lv_event_t* event){
		TextEntry* textEntry = static_cast<ProfileScreen*>(lv_event_get_user_data(event))->name;
		if(textEntry->isActive()){
			textEntry->stop();
			lv_group_focus_freeze((lv_group_t*)(lv_obj_get_group(textEntry->getLvObj())), false);
			lv_obj_set_style_bg_opa(textEntry->getLvObj(), LV_OPA_0, 0);
		}
		else{
			textEntry->start();
			lv_group_focus_freeze((lv_group_t*)(lv_obj_get_group(textEntry->getLvObj())), true);
			lv_obj_set_style_bg_opa(textEntry->getLvObj(), LV_OPA_100, 0);
		}
	}, LV_EVENT_CLICKED, this);
	lv_obj_set_height(header, LV_SIZE_CONTENT);

}

void ProfileScreen::buildBody(){
	lv_obj_t* body = lv_obj_create(obj);
	lv_obj_set_layout(body, LV_LAYOUT_FLEX);
	lv_obj_set_style_bg_opa(body, 0, 0);
	lv_obj_set_flex_flow(body, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(body, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(body, 3, 0);
	lv_obj_set_style_pad_ver(body, 5, 0);
	lv_obj_set_style_pad_hor(body, 1, 0);
	lv_obj_set_scrollbar_mode(body, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_width(body, lv_pct(100));
	lv_obj_set_style_border_width(body, 1, 0);
	lv_obj_set_style_border_color(body, lv_color_white(), 0);
	lv_obj_set_style_border_side(body, LV_BORDER_SIDE_BOTTOM, 0);

	editableAvatar = new EditableAvatar(body, profile.avatar, true);
	if(editable){
		lv_group_add_obj(inputGroup, editableAvatar->getLvObj());
	}

	lv_obj_t* colorObj = lv_obj_create(body);
	lv_obj_set_flex_flow(colorObj, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(colorObj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(colorObj, 3, 0);
	lv_obj_set_style_bg_opa(colorObj, 0, 0);
/*	lv_obj_set_style_border_width(colorObj, 1, 0);
	lv_obj_set_style_border_color(colorObj, lv_color_white(), 0);
	lv_obj_set_style_border_opa(colorObj, LV_OPA_100, 0);*/

	lv_obj_t* colorLabel = lv_label_create(colorObj);
	lv_obj_add_style(colorLabel, &textStyle, 0);
	lv_label_set_text(colorLabel, "Color");

	cbox = new ColorBox(colorObj, profile.color);
	if(editable){
		lv_group_add_obj(inputGroup, cbox->getLvObj());
	}
	lv_obj_set_size(colorObj, lv_pct(60), LV_SIZE_CONTENT);

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
	//TODO - maknuti placeholdere, dohvatiti iz storagea
	char buf[50];
	sprintf(buf, "Friends : #f4b41b %d #", 2);
	lv_label_set_text(friends, buf);
	sprintf(buf, "Messages received : #f4b41b %d #", 2223);
	lv_label_set_text(received, buf);
	sprintf(buf, "Messages sent : #f4b41b %d #", 29998);
	lv_label_set_text(sent, buf);

	lv_obj_set_height(footer, LV_SIZE_CONTENT);

}

void ProfileScreen::onStart(){
	if(editable){
		Input::getInstance()->addListener(this);
	}
}

void ProfileScreen::onStop(){
	if(editable){
		Input::getInstance()->removeListener(this);
		name->getText();
		editableAvatar->getIndex();
		cbox->getHue();
		//TODO - staviti pisanje ovoga u storage
	}
}

void ProfileScreen::buttonPressed(uint i){
	if(!editable) return;
	if(i == BTN_BACK){
		if(!name->isActive()){
			pop();
			return;
		}else{
			lv_obj_set_style_bg_opa(name->getLvObj(), LV_OPA_0, 0);
			lv_group_focus_freeze((lv_group_t*)(lv_obj_get_group(name->getLvObj())), false);
		}
		return;
	}

	if(i != BTN_LEFT && i != BTN_RIGHT && i != BTN_ENTER){
		if(name->isActive()) return;
		name->start();
		name->keyPress(i);
	}
}
