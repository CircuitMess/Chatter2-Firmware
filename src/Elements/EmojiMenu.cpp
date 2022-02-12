#include "EmojiMenu.h"
#include <Arduino.h>
#include "../InputLVGL.h"

EmojiMenu::EmojiMenu(lv_obj_t* parent, void(* callback)(uint8_t emojiID, void* userData), void* userData) : LVObject(parent), returnCallback(callback),
																											userData(userData){
	lv_obj_set_style_bg_color(obj, lv_color_make(57, 49, 75), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_border_width(obj, 1, 0);
	lv_obj_set_style_border_color(obj, lv_color_make(142, 71, 140), 0);
	lv_obj_set_style_border_opa(obj, LV_OPA_100, 0);

	static lv_coord_t col_dsc[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

	lv_obj_set_layout(obj, LV_LAYOUT_GRID);
	lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
	lv_obj_set_grid_align(obj, LV_GRID_ALIGN_SPACE_BETWEEN, LV_GRID_ALIGN_SPACE_BETWEEN);
	lv_obj_set_size(obj, 149, 78);
	lv_obj_set_style_pad_ver(obj, 4, 0);
	lv_obj_set_style_pad_hor(obj, 9, 0);

	group = lv_group_create();
	char path[20];
	for(int i = 1; i < 25; ++i){
		sprintf(path, "S:/Emojis/%d.bin", i);
		auto img = lv_img_create(obj);
		lv_img_set_src(img, path);

		lv_obj_add_event_cb(img, [](lv_event_t* e){
			static_cast<EmojiMenu*>(lv_event_get_user_data(e))->exit(lv_obj_get_index(lv_event_get_target(e)) + 1);
		}, LV_EVENT_CLICKED, this);

		lv_obj_add_event_cb(img, [](lv_event_t* e){
			if(lv_event_get_key(e) == LV_KEY_ESC){
				static_cast<EmojiMenu*>(lv_event_get_user_data(e))->exit(0xFF);
			}
		}, LV_EVENT_KEY, this);

		lv_obj_set_style_outline_pad(img, 1, LV_STATE_FOCUSED);
		lv_obj_set_style_outline_color(img, lv_color_white(), LV_STATE_FOCUSED);
		lv_obj_set_style_outline_opa(img, LV_OPA_100, LV_STATE_FOCUSED);
		lv_obj_set_style_outline_width(img, 1, LV_STATE_FOCUSED);
		lv_group_add_obj(group, img);
		lv_obj_clear_flag(img, LV_OBJ_FLAG_SCROLLABLE);
		lv_obj_set_grid_cell(img, LV_GRID_ALIGN_CENTER, (i - 1) % 6, 1, LV_GRID_ALIGN_CENTER, (i - 1) / 6, 1);
	}
}

void EmojiMenu::enter(lv_group_t* oldGroup){
	this->oldGroup = oldGroup;
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), group);
}

void EmojiMenu::exit(uint8_t emojiID){
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), oldGroup);
	returnCallback(emojiID, userData);
}
