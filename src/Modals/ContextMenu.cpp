#include "ContextMenu.h"
#include <lvgl.h>
#include "../InputLVGL.h"
#include "../Fonts/font.h"

ContextMenu::ContextMenu(LVScreen* parent, const std::vector<Option>& options) : LVModal(parent){
	lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_style_pad_all(obj, 1, 0);
	lv_obj_set_style_pad_gap(obj, 1, 0);
	lv_obj_set_style_bg_color(obj, lv_color_white(), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);

	lv_style_init(&styleDef);
	lv_style_set_bg_opa(&styleDef, LV_OPA_COVER);
	lv_style_set_text_font(&styleDef, &pixelbasic7);
	lv_style_set_text_color(&styleDef, lv_color_white());
	lv_style_set_bg_color(&styleDef,lv_color_hsv_to_rgb(277, 36, 39));
	lv_style_set_pad_hor(&styleDef, 3);
	lv_style_set_pad_ver(&styleDef, 4);

	lv_style_init(&styleFocus);
	lv_style_set_bg_color(&styleFocus, lv_color_hsv_to_rgb(332, 53, 80));
	lv_style_set_border_opa(&styleFocus, LV_OPA_COVER);
	lv_style_set_border_width(&styleFocus, 1);
	lv_style_set_border_color(&styleFocus, lv_color_white());
	lv_style_set_pad_hor(&styleFocus, 2);
	lv_style_set_pad_ver(&styleFocus, 3);

	lv_obj_set_width(obj, 110);
	lv_obj_set_height(obj, LV_SIZE_CONTENT);

	setOptions(options);

	lv_obj_add_event_cb(obj, [](lv_event_t* e){
		auto* menu = static_cast<ContextMenu*>(e->user_data);
		menu->stop();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(obj, [](lv_event_t* e){
		auto* menu = static_cast<ContextMenu*>(e->user_data);
		menu->stop();
	}, LV_EVENT_CANCEL, this);
}

ContextMenu::~ContextMenu(){
	lv_style_reset(&styleFocus);
	lv_style_reset(&styleDef);
}

void ContextMenu::setOptions(const std::vector<Option>& options){
	this->options = options;

	for(auto* label : labels){
		lv_obj_del(label);
	}
	labels.clear();

	for(const auto& option : options){
		label = lv_label_create(obj);
		labels.push_back(label);

		lv_obj_clear_flag(label, LV_OBJ_FLAG_CLICK_FOCUSABLE);
		lv_obj_clear_flag(label, LV_OBJ_FLAG_CHECKABLE);
		lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);

		lv_label_set_text(label, option.text.c_str());

		lv_obj_set_width(label, lv_pct(100));
		lv_obj_set_height(label, LV_SIZE_CONTENT);

		lv_obj_add_style(label, &styleFocus, LV_STATE_FOCUSED);
		lv_obj_add_style(label, &styleDef, LV_STATE_DEFAULT);

		lv_group_add_obj(inputGroup, label);
		lv_obj_add_flag(label, LV_OBJ_FLAG_EVENT_BUBBLE);
	}
}

const ContextMenu::Option& ContextMenu::getSelected(){
	lv_obj_t* focused = lv_group_get_focused(inputGroup);
	if(!focused) return { };

	uint32_t index = lv_obj_get_index(focused);
	if(index >= options.size()) return { };

	return options[index];
}

void ContextMenu::onStart(){
	lv_group_focus_obj(labels.front());
}
