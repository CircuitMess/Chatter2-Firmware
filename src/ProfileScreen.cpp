#include "ProfileScreen.h"
#include "ColorBox.h"
#include "EditableAvatar.h"
#include <lvgl.h>
#include "font.h"

ProfileScreen::ProfileScreen() : LVScreen(){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(obj, 5, 0);

	lv_style_selector_t screenSel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_init(&screenStyle);
	lv_obj_add_style(obj, &screenStyle, screenSel);

	lv_obj_set_style_bg_img_src(obj,"S:/bg.bin",screenSel);

	lv_obj_set_style_border_width(obj, 1, screenSel);
	lv_obj_set_style_border_color(obj, lv_color_hsv_to_rgb(0,0,100), screenSel);
//header
	lv_style_selector_t profileSel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t nameSel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t nameSelFocus = LV_PART_MAIN | LV_STATE_FOCUSED;

	lv_obj_t * header = lv_obj_create(obj);
	lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(header, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(header,1,0);
	lv_obj_set_style_bg_opa(header,LV_OPA_0,0);

	lv_obj_t* profileTag = lv_obj_create(header);

	lv_obj_set_flex_align(profileTag, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	name = lv_textarea_create(header);
	lv_group_add_obj(inputGroup, name);
	lv_obj_set_style_opa(name, LV_OPA_0, LV_PART_CURSOR | LV_STATE_FOCUSED);
	lv_obj_set_style_opa(name, LV_OPA_100, LV_PART_CURSOR | LV_STATE_EDITED);
	lv_obj_set_style_bg_opa(name,LV_OPA_100,LV_STATE_EDITED);
	lv_textarea_set_one_line(name, true);
	lv_textarea_set_max_length(name,15);

	lv_obj_add_event_cb(name, [](lv_event_t* event){
		uint32_t c = lv_event_get_key(event);
		auto profileEvent = static_cast<ProfileScreen*>(event->user_data);
		if(c == LV_KEY_ESC){
			profileEvent->exit();
		}
	}, LV_EVENT_KEY, this);

	lv_obj_set_style_bg_opa(profileTag,0,0);
	lv_obj_set_style_bg_opa(name,0,0);

	lv_style_init(&profile);
	lv_obj_add_style(profileTag, &profile, profileSel);

	lv_obj_set_width(header, lv_pct(100));
	lv_obj_set_height(header,LV_SIZE_CONTENT);
	lv_obj_set_width(profileTag, lv_pct(25));
	lv_obj_set_height(profileTag,LV_SIZE_CONTENT);
	lv_obj_set_width(name, lv_pct(74));
	lv_obj_set_height(name,LV_SIZE_CONTENT);

	lv_obj_set_style_pad_all(header,1,0);

	lv_obj_set_style_border_width(name, 1, nameSel);
	lv_obj_set_style_border_color(name, lv_color_hsv_to_rgb(0,0,100), nameSel);
	lv_obj_set_style_border_opa(name,0,nameSel);
	lv_obj_set_style_pad_all(name,1,nameSel);

	lv_obj_set_style_border_width(name, 1, nameSelFocus);
	lv_obj_set_style_border_color(name, lv_color_hsv_to_rgb(0,0,100), nameSelFocus);
	lv_obj_set_style_border_opa(name,LV_OPA_100,nameSelFocus);
	lv_obj_set_style_pad_all(name,1,nameSelFocus);

	lv_obj_t* label = lv_label_create(profileTag);
	lv_style_set_text_font(&profile, &pixelbasic24);
	lv_label_set_recolor(label,true);
	lv_label_set_text(label, "#ffffff Profile");
	//lv_obj_set_flex_grow(label, 1);

	lv_style_init(&nameStyleDef);
	lv_obj_add_style(name, &nameStyleDef, 0);
	char nick[15];
	lv_style_set_text_font(&nameStyleDef, &pixelnickname24);
	lv_obj_set_style_text_color(name,lv_color_hsv_to_rgb(21,91,100),0);
	sprintf(nick,"%s","MOMOMOMOMOMO");
	lv_textarea_set_text(name,nick);
	//lv_obj_set_flex_grow(nick, 1);

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;
//body
	lv_obj_t * obj1 = lv_obj_create(obj);

	lv_obj_t * obj2 = lv_obj_create(obj);

	lv_obj_set_layout(obj1, LV_LAYOUT_FLEX);
	lv_style_init(&styleDef);
	lv_obj_add_style(obj1, &styleDef, sel);
	lv_obj_set_style_bg_opa(obj1,0,sel);
	lv_style_init(&styleFocus);

	lv_obj_set_flex_flow(obj1, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(obj1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(obj1, 5, sel);

	//lv_obj_set_style_pad_bottom(obj1,1,sel);

	auto editableAvatar = new EditableAvatar(obj1,1,true);
	lv_group_add_obj(inputGroup, editableAvatar->getLvObj());

	lv_obj_t * colorObj = lv_obj_create(obj1);
	lv_obj_add_style(colorObj,&styleDef,0);
	lv_style_set_text_font(&styleDef, &pixelbasic24);
	lv_obj_set_flex_flow(colorObj, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(colorObj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(colorObj, 3, 0);
	lv_obj_set_style_bg_opa(colorObj,0,0);

	lv_obj_t* colorLabel = lv_label_create(colorObj);
	lv_label_set_recolor(colorLabel,true);
	lv_label_set_text(colorLabel, "#ffffff Colour");

	auto cBox = new ColorBox(colorObj,0);
	lv_group_add_obj(inputGroup, cBox->getLvObj());

	lv_obj_set_width(obj1, lv_pct(100));
	lv_obj_set_height(obj1,LV_SIZE_CONTENT);
	lv_obj_set_width(colorObj, lv_pct(60));
	lv_obj_set_height(colorObj,LV_SIZE_CONTENT);
	lv_style_init(&styleDef2);
	lv_obj_add_style(obj2, &styleDef2, sel);
//footer
	lv_obj_set_flex_flow(obj2, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj2, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_style_pad_gap(obj2, 6, sel);

	lv_style_init(&styleFocus2);
	lv_obj_add_style(obj2, &styleFocus2, selFocus);
	lv_obj_set_width(obj2, lv_pct(100));
	lv_obj_set_style_bg_opa(obj2,0,0);
	lv_obj_set_flex_grow(obj2,1);

	lv_style_set_text_font(&styleDef2, &pixelbasic24);
	lv_obj_t* friends = lv_label_create(obj2);
	lv_obj_t* received = lv_label_create(obj2);
	lv_obj_t* sent = lv_label_create(obj2);
	char friendsBuf[50];
	char receivedBuf[50];
	char sentBuf[50];
	lv_label_set_recolor(friends,true);
	sprintf(friendsBuf,"#ffffff Friends : %d",2);
	lv_label_set_text(friends, friendsBuf);
	sprintf(receivedBuf,"#ffffff Messages received : %d",2223);
	lv_label_set_recolor(received,true);
	lv_label_set_text(received, receivedBuf);
	sprintf(sentBuf,"#ffffff Messages sent : %d",29998);
	lv_label_set_recolor(sent,true);
	lv_label_set_text(sent, sentBuf);

	lv_obj_set_style_border_width(obj1, 1, sel);
	lv_obj_set_style_border_color(obj1, lv_color_hsv_to_rgb(0,0,100), sel);
	lv_obj_set_style_border_side(obj1, LV_BORDER_SIDE_BOTTOM,sel);
	lv_obj_set_style_border_width(header, 1, nameSel);
	lv_obj_set_style_border_color(header, lv_color_hsv_to_rgb(0,0,100), nameSel);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM,nameSel);

}

void ProfileScreen::exit(){
	Serial.println("EXIT");
	lv_group_set_editing((lv_group_t*)lv_obj_get_group(name), false);
	lv_obj_add_state(name,LV_PART_MAIN | LV_STATE_DEFAULT);
}