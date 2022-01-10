#include "Profile.h"
#include <lvgl.h>

Profile::Profile() : LVScreen(){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(obj, 5, 0);

	lv_style_selector_t screenSel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_init(&screenStyle);
	lv_obj_add_style(obj, &screenStyle, screenSel);

	lv_obj_set_style_border_width(obj, 1, screenSel);
	lv_obj_set_style_border_color(obj, lv_color_hsv_to_rgb(0,0,100), screenSel);

	//name
	lv_style_selector_t profileSel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t nameSel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t nameSelFocus = LV_PART_MAIN | LV_STATE_FOCUSED;

	lv_obj_t * header = lv_obj_create(obj);
	lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(header, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

	lv_obj_t* profileTag = lv_obj_create(header);
	lv_obj_t* name = lv_obj_create(header);

	lv_style_init(&profile);
	lv_obj_add_style(profileTag, &profile, profileSel );

	lv_obj_set_width(header, lv_pct(100));
	lv_obj_set_height(header,LV_SIZE_CONTENT);
	lv_obj_set_width(profileTag, lv_pct(25));
	lv_obj_set_height(profileTag,LV_SIZE_CONTENT);
	lv_obj_set_width(name, lv_pct(75));
	lv_obj_set_height(name,LV_SIZE_CONTENT);

	lv_obj_set_style_pad_all(header,2,0);
	/*lv_style_init(&nameStyleFocus);
	lv_obj_add_style(name, &nameStyleFocus, nameSelFocus);*/

	lv_obj_set_style_border_width(name, 1, nameSelFocus);
	lv_obj_set_style_border_color(name, lv_color_hsv_to_rgb(0,0,100), nameSelFocus);

	lv_obj_t* label = lv_label_create(profileTag);
	lv_style_set_text_font(&profile, &pixelbasic24);
	lv_label_set_recolor(label,true);
	lv_label_set_text(label, "#ffffff Profile");
	lv_obj_set_flex_grow(label, 1);

	lv_style_init(&nameStyleDef);
	lv_obj_add_style(name, &nameStyleDef, nameSel);

	lv_obj_t* nick = lv_label_create(name);
	lv_style_set_text_font(&nameStyleDef, &pixelnickname24);
	lv_label_set_recolor(nick,true);
	lv_label_set_text(nick, "#ff3700 MOMOMOMOMOM");
	lv_obj_set_flex_grow(nick, 1);

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;

	lv_obj_t * obj1 = lv_obj_create(obj);

	lv_obj_t * obj2 = lv_obj_create(obj);

	lv_obj_set_layout(obj1, LV_LAYOUT_FLEX);
	lv_style_init(&styleDef);
	lv_obj_add_style(obj1, &styleDef, sel);

	// Focused style
	lv_style_init(&styleFocus);
	lv_obj_add_style(obj1, &styleFocus, selFocus);

	lv_obj_set_flex_flow(obj1, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(obj1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(obj1, 30, sel);

	lv_obj_set_style_pad_bottom(obj1,5,sel);

	lv_obj_t* avatar = lv_obj_create(obj1);

	lv_obj_set_size(avatar, 42, 42);
	lv_obj_set_style_bg_color(avatar, lv_color_hsv_to_rgb(0, 100, 100), sel);
	lv_obj_set_style_radius(avatar, LV_RADIUS_CIRCLE, sel);


	lv_obj_t* colorBox = lv_obj_create(obj1);
	lv_obj_set_size(colorBox, 60, 20);
	lv_obj_set_style_bg_color(colorBox, lv_color_hsv_to_rgb(0, 100, 100), sel);

	lv_obj_set_width(obj1, lv_pct(100));
	lv_obj_set_height(obj1,LV_SIZE_CONTENT);

	lv_style_init(&styleDef2);
	lv_obj_add_style(obj2, &styleDef2, sel);

	lv_obj_set_flex_flow(obj2, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj2, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_style_pad_gap(obj2, 6, sel);
	// Focused style
	lv_style_init(&styleFocus2);
	lv_obj_add_style(obj2, &styleFocus2, selFocus);
	lv_obj_set_width(obj2, lv_pct(100));

	lv_obj_set_flex_grow(obj2,1);

	lv_style_set_text_font(&styleDef2, &pixelbasic24);
	lv_obj_t* friends = lv_label_create(obj2);
	lv_obj_t* received = lv_label_create(obj2);
	lv_obj_t* sent = lv_label_create(obj2);
	lv_label_set_recolor(friends,true);
	lv_label_set_text(friends, "#ffffff Friends :");
	lv_label_set_recolor(received,true);
	lv_label_set_text(received, "#ffffff Messages received :");
	lv_label_set_recolor(sent,true);
	lv_label_set_text(sent, "#ffffff Messages sent :");

	lv_obj_set_style_border_width(obj1, 1, sel);
	lv_obj_set_style_border_color(obj1, lv_color_hsv_to_rgb(0,0,100), sel);
	lv_obj_set_style_border_side(obj1, LV_BORDER_SIDE_BOTTOM,sel);
	lv_obj_set_style_border_width(header, 1, nameSel);
	lv_obj_set_style_border_color(header, lv_color_hsv_to_rgb(0,0,100), nameSel);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM,nameSel);



}
