#include "ContextMenu.h"
#include <lvgl.h>
#include "../InputLVGL.h"
#include "../font.h"

ContextMenu::ContextMenu(LVScreen* parent, const std::vector<std::string>& options) : LVModal(parent){
	lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

	lv_style_init(&styleDef);
	lv_style_set_bg_opa(&styleDef, LV_OPA_COVER);
	lv_style_set_text_font(&styleDef, &pixelbasic_7);
	lv_style_set_text_color(&styleDef, lv_color_white());
	lv_style_set_bg_color(&styleDef,lv_color_hsv_to_rgb(326, 20, 64));
	lv_style_set_border_color(&styleDef, lv_color_white());
	lv_style_set_border_width(&styleDef, 1);
	lv_style_set_pad_hor(&styleDef, 2);
	lv_style_set_pad_ver(&styleDef, 3);

	lv_style_init(&styleFocus);
	lv_style_set_bg_color(&styleFocus, lv_color_hsv_to_rgb(325, 91, 55));
	lv_style_set_border_width(&styleFocus, 2);
	lv_style_set_pad_hor(&styleFocus, 1);
	lv_style_set_pad_ver(&styleFocus, 2);

	lv_obj_set_width(obj, lv_pct(70));
	lv_obj_set_height(obj, LV_SIZE_CONTENT);

	for(const auto& text : options){
		labelObj = lv_label_create(obj);
		labelVector.push_back(labelObj);
		lv_label_set_text(labelObj, text.c_str());

		lv_obj_set_width(labelObj, lv_pct(100));
		lv_obj_set_height(labelObj, LV_SIZE_CONTENT);

		lv_obj_add_style(labelObj, &styleFocus, LV_STATE_FOCUSED);
		lv_obj_add_style(labelObj, &styleDef, LV_STATE_DEFAULT);

		lv_group_add_obj(inputGroup, labelObj);
	}

}


