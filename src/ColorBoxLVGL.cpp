#include "ColorBoxLVGL.h"


ColorBox::ColorBox(lv_obj_t* parent, uint16_t hue) : LVObject(parent), color(color){
	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;
	lv_style_selector_t selPress = LV_PART_MAIN | LV_STATE_PRESSED;

	lv_obj_set_size(obj,50,20);

	static const lv_style_prop_t props[] = {LV_STYLE_BG_COLOR};
	static lv_style_transition_dsc_t transition_dsc;
	lv_style_transition_dsc_init(&transition_dsc, props, lv_anim_path_linear, 300, 0, NULL);

	static lv_style_t style_main;
	static lv_style_t style_knob;
	static lv_style_t style_pressed_color;
	
	lv_style_init(&style_main);
	lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
	lv_style_set_bg_img_src(&style_main, "S:/colorPicker.bin");

	lv_style_init(&style_knob);
	lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
	lv_style_set_bg_img_src(&style_knob, "S:/arrowKnob.bin");

	/*Create a slider and add the style*/
	lv_obj_t * slider = lv_slider_create(obj);
	lv_obj_remove_style_all(slider);        /*Remove the styles coming from the theme*/
	lv_obj_set_width(slider,50);
	lv_obj_set_height(slider,10);
	lv_obj_add_style(slider, &style_main, LV_PART_MAIN);
	lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);
	lv_slider_set_value(slider,20,LV_ANIM_OFF);
	//lv_obj_add_style(slider, &style_pressed_color, LV_PART_KNOB | LV_STATE_PRESSED);
	lv_obj_center(slider);

	lv_obj_set_style_translate_y(slider, 5,LV_PART_KNOB);

//------------------------------------------

	/*lv_obj_t* img = lv_img_create(obj);
	lv_img_set_src(img, "S:/colorPicker.bin");
	lv_obj_set_style_border_width(img, 1, LV_STATE_DEFAULT);
	lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(img, 46, 8);*/

	/*lv_style_init(&styleDef);
	lv_style_set_bg_color(&styleDef, color);
	lv_obj_add_style(obj, &styleDef, sel);

	lv_style_init(&styleFocus);
	//lv_style_set_bg_color(&styleFocus, lv_color_hsv_to_rgb(hue,60,60));
	lv_obj_add_style(obj, &styleFocus, selFocus);*/


}
