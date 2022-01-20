#include "ContextMenu.h"
#include <lvgl.h>
#include "InputLVGL.h"

ContextMenu::ContextMenu(lv_obj_t* parent, const std::vector<std::string>& options) : LVObject(parent){

	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_style_bg_color(obj, lv_color_hsv_to_rgb(0, 0, 50), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_border_width(obj, 1, 0);
	lv_obj_set_style_border_color(obj, lv_color_white(), 0);

	// Selectors
	lv_style_selector_t def = LV_STATE_DEFAULT;
	lv_style_selector_t focus = LV_STATE_FOCUSED;

	lv_style_init(&styleDef);
	lv_style_set_bg_color(&styleDef, lv_color_hsv_to_rgb(0, 100, 100));
	lv_style_set_bg_opa(&styleDef, LV_OPA_COVER);

	lv_style_init(&styleFocus);
	lv_style_set_bg_color(&styleFocus, lv_color_hsv_to_rgb(0, 20, 100));
	lv_style_set_bg_opa(&styleFocus, LV_OPA_COVER);

	// width / height
	lv_obj_set_width(obj, lv_pct(70));
	lv_obj_set_height(obj, LV_SIZE_CONTENT);

	for(int i = 0; i < options.size(); ++i){
		lv_style_set_text_font(&styleFocus, &pixelbasic22);
		lv_style_set_text_font(&styleDef, &pixelbasic22);
		labelObj = lv_label_create(obj);
		lv_label_set_text(labelObj, options[i].c_str());
		labelVector.push_back(labelObj);

		lv_group_add_obj((lv_group_t*) lv_obj_get_group(obj), labelObj);
		lv_obj_set_width(labelObj, lv_pct(100));
		lv_obj_set_height(labelObj, LV_SIZE_CONTENT);
		lv_obj_set_style_pad_all(labelObj, 2, 0);

		if(i == 0){
			lv_obj_set_style_border_width(labelObj, 1, 0);
			lv_obj_set_style_border_color(labelObj, lv_color_white(), 0);
			lv_obj_set_style_border_side(labelObj, LV_BORDER_SIDE_BOTTOM, 0);
			lv_obj_add_style(labelObj, &styleFocus, LV_STATE_FOCUSED);
			lv_obj_add_style(labelObj, &styleDef, def);

		}else{
			if(i == (options.size()-1)){
				lv_obj_set_style_border_width(labelObj, 0, 0);
			}else{
				lv_obj_set_style_border_width(labelObj, 1, 0);
			}
			lv_obj_set_style_border_color(labelObj, lv_color_white(), 0);
			lv_obj_set_style_border_side(labelObj, LV_BORDER_SIDE_BOTTOM, 0);
			lv_obj_add_style(labelObj, &styleFocus, LV_STATE_FOCUSED);
			lv_obj_add_style(labelObj, &styleDef, def);
		}
	}

}

