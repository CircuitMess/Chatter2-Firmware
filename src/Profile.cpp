#include "Profile.h"

Profile::Profile() : LVScreen(){
	lv_obj_set_height(screen, LV_SIZE_CONTENT);
	lv_obj_set_layout(screen,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(screen, 5, 0);

	lv_style_selector_t screenSel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_init(&screenStyle);
	lv_obj_add_style(screen, &screenStyle, screenSel);

	lv_obj_set_style_border_width(screen, 1, screenSel);
	lv_obj_set_style_border_color(screen, lv_color_hsv_to_rgb(0,0,100), screenSel);

	//name
	lv_style_selector_t nameSel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t nameSelFocus = LV_PART_MAIN | LV_STATE_FOCUSED;

	lv_obj_t * name = lv_obj_create(screen);

	lv_style_init(&nameStyleDef);
	lv_obj_add_style(name, &nameStyleDef, nameSel);

	lv_color_t nameColor = lv_color_hsv_to_rgb(120,100,100);
	lv_style_set_bg_color(&nameStyleDef, nameColor);

	lv_style_init(&nameStyleFocus);
	lv_style_set_bg_color(&nameStyleFocus, nameColor);
	lv_obj_add_style(name, &nameStyleFocus, nameSelFocus);

	lv_obj_set_style_border_width(name, 1, nameSelFocus);
	lv_obj_set_style_border_color(name, lv_color_hsv_to_rgb(0,0,100), nameSelFocus);

	lv_obj_t* label = lv_label_create(name);
	lv_label_set_text(label, "Profile");
	lv_obj_set_flex_grow(label, 1);

	lv_obj_set_width(name, lv_pct(100));
	lv_obj_set_height(name,LV_SIZE_CONTENT);

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;

	lv_obj_t * obj1 = lv_obj_create(screen);

	lv_obj_t * obj2 = lv_obj_create(screen);

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

	lv_obj_set_flex_flow(obj2, LV_FLEX_FLOW_COLUMN);

	lv_style_init(&styleDef2);
	lv_obj_add_style(obj2, &styleDef2, sel);

	// Focused style
	lv_style_init(&styleFocus2);
	lv_obj_add_style(obj2, &styleFocus2, selFocus);
	lv_obj_set_width(obj2, lv_pct(100));
	lv_obj_set_flex_grow(obj2,1);

	lv_obj_t* stats = lv_label_create(obj2);
	lv_label_set_recolor(stats,true);
	lv_label_set_text(stats, "#ffffff Friends :\n"
							      "#ffffff Messages received :\n"
								   "#ffffff Messages sent :");

	lv_obj_set_style_border_width(obj1, 1, sel);
	lv_obj_set_style_border_color(obj1, lv_color_hsv_to_rgb(0,0,100), sel);
	lv_obj_set_style_border_side(obj1, LV_BORDER_SIDE_BOTTOM,sel);



}
