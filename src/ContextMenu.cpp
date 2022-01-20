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
	lv_style_set_bg_color(&styleDef,lv_color_hsv_to_rgb(326, 20, 64));
	lv_style_set_bg_opa(&styleDef, LV_OPA_COVER);

	lv_style_init(&styleFocus);
	lv_style_set_bg_color(&styleFocus, lv_color_hsv_to_rgb(325, 91, 55));
	lv_style_set_bg_opa(&styleFocus, LV_OPA_COVER);

	// width / height
	lv_obj_set_width(obj, lv_pct(70));
	lv_obj_set_height(obj, LV_SIZE_CONTENT);

	for(int i = 0; i < options.size(); ++i){
		lv_style_set_text_font(&styleFocus, &pixelbasic22);
		lv_style_set_text_font(&styleDef, &pixelbasic22);
		labelObj = lv_label_create(obj);
		char contextMessage[25];
		lv_label_set_recolor(labelObj,true);
		sprintf(contextMessage,"#ffffff %s",options[i].c_str());
		lv_label_set_text(labelObj, contextMessage);
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
			lv_obj_add_state(labelObj,LV_STATE_FOCUSED);
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

	lv_group_add_obj((lv_group_t*) lv_obj_get_group(obj), labelVector[num]);

	/*lv_obj_add_event_cb(obj, [](lv_event_t* event){
		static_cast<ContextMenu*>(lv_event_get_user_data(event))->down();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		static_cast<ContextMenu*>(lv_event_get_user_data(event))->up();
	}, LV_EVENT_LEAVE, this);*/

	lv_obj_add_event_cb(obj, [](lv_event_t* event){
		uint32_t c = lv_event_get_key(event);
		auto menu = static_cast<ContextMenu*>(event->user_data);
		switch(c){
			case LV_KEY_RIGHT:
				menu->down();
				break;
			case LV_KEY_LEFT:
				menu->up();
				break;
			default:
				break;
		}
	}, LV_EVENT_KEY, this);

}

void ContextMenu::down(){
	if(num == labelVector.size()-1){
		lv_obj_clear_state(labelVector[labelVector.size()-1],LV_STATE_FOCUSED);
		lv_obj_add_state(labelVector[labelVector.size()-1],LV_STATE_DEFAULT);
		num = 0;
	}else{
		lv_obj_clear_state(labelVector[num],LV_STATE_FOCUSED);
		lv_obj_add_state(labelVector[num],LV_STATE_DEFAULT);
		Serial.println(num);
		num++;
	}
	lv_obj_add_state(labelVector[num],LV_STATE_FOCUSED);
}

void ContextMenu::up(){
	if(num == 0){
		lv_obj_clear_state(labelVector[num],LV_STATE_FOCUSED);
		lv_obj_add_state(labelVector[num],LV_STATE_DEFAULT);
		num = labelVector.size()-1;
	}else{
		lv_obj_clear_state(labelVector[num],LV_STATE_FOCUSED);
		lv_obj_add_state(labelVector[num],LV_STATE_DEFAULT);
		Serial.println(num);
		num--;
	}
	lv_obj_add_state(labelVector[num],LV_STATE_FOCUSED);

}

