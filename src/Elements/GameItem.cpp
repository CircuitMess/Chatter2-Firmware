#include "GameItem.h"
#include "../Fonts/font.h"

GameItem::GameItem(lv_obj_t* parent, const std::string& text, const std::string& icon) : LVObject(parent){
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(obj, 7, 0);

	// Selectors
	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;

	// width / height
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_width(obj, lv_pct(100));

	// Default style
	lv_style_init(&styleDef);
	lv_style_set_bg_opa(&styleDef,0);
	lv_style_set_border_color(&styleDef, lv_color_white());
	lv_style_set_border_opa(&styleDef, LV_OPA_100);
	lv_style_set_border_width(&styleDef, 1);
	lv_style_set_pad_all(&styleDef, 2);
	lv_obj_add_style(obj, &styleDef, sel);

	// Focused style
	lv_style_init(&styleFocus);
	lv_style_set_pad_all(&styleFocus, 1);
	lv_style_set_border_width(&styleFocus, 2);
	lv_style_set_bg_opa(&styleFocus, LV_OPA_100);
	lv_style_set_bg_color(&styleFocus, lv_color_make(86, 64, 100));
	lv_obj_add_style(obj, &styleFocus, selFocus);

	image = lv_img_create(obj);
	lv_img_set_src(image, icon.c_str());

	// Name
	name = lv_label_create(obj);
	lv_obj_set_style_text_font(name, &pixelbasic7, sel);
	lv_obj_set_style_text_color(name, lv_color_white(), sel);
	lv_label_set_text(name, text.c_str());
	lv_obj_set_flex_grow(name, 1);
	lv_obj_set_style_pad_top(name, 1, 0);
}

GameItem::~GameItem(){
	lv_style_reset(&styleDef);
	lv_style_reset(&styleFocus);
}
