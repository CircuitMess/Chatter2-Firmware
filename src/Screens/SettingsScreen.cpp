#include "SettingsScreen.h"
#include <Settings.h>
#include <string>
#include "../Fonts/font.h"
#include "UserHWTest.h"
#include <Input/Input.h>
#include <Pins.hpp>
#include <Chatter.h>
#include "../Services/SleepService.h"
#include "../Storage/Storage.h"
#include "../Modals/Prompt.h"
#include <Audio/Piezo.h>
#include "../Services/BuzzerService.h"
#include "LockScreen.h"
#include <Notes.h>
#include <nvs_flash.h>


SettingsScreen::SettingsScreen() : LVScreen(){

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;
	lv_style_selector_t selPress = LV_STATE_EDITED;

	lv_style_init(&style_def);
	lv_style_set_border_width(&style_def, 1);
	lv_style_set_border_opa(&style_def, 0);

	lv_style_init(&style_focused);
	lv_style_set_border_width(&style_focused, 1);
	lv_style_set_border_color(&style_focused, lv_color_white());
	lv_style_set_border_opa(&style_focused, LV_OPA_COVER);

	lv_style_init(&style_pressed);
	lv_style_set_border_width(&style_pressed, 1);
	lv_style_set_border_color(&style_pressed, lv_color_hex(0x892eff));
	lv_style_set_border_opa(&style_pressed, LV_OPA_COVER);

	lv_obj_set_size(obj, lv_pct(100), LV_SIZE_CONTENT);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_gap(obj, 2, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);

	//Version
	version = lv_obj_create(obj);
	lv_obj_set_height(version, LV_SIZE_CONTENT);
	lv_obj_set_width(version, lv_pct(100));
	lv_obj_set_layout(version, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(version, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(version, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(version, 8, 0);
	lv_obj_set_style_pad_all(version, 3, 0);
	lv_obj_set_style_bg_opa(version, 0, 0);


	lv_obj_t* versionLabel = lv_label_create(version);
	lv_obj_set_style_text_font(versionLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(versionLabel, lv_color_white(), 0);
	lv_label_set_text(versionLabel, "Chatter 2.0 Settings");

	//sound ON/OFF
	sound = lv_obj_create(obj);
	lv_obj_set_height(sound, LV_SIZE_CONTENT);
	lv_obj_set_width(sound, lv_pct(100));
	lv_obj_set_layout(sound, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(sound, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(sound, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(sound, 80, 0);
	lv_obj_set_style_pad_all(sound, 3, 0);
	lv_obj_set_style_bg_opa(sound, 0, 0);
	lv_obj_add_style(sound, &style_focused, selFocus);
	lv_obj_add_style(sound, &style_def, sel);

	lv_obj_t* soundLabel = lv_label_create(sound);
	lv_obj_set_style_text_font(soundLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(soundLabel, lv_color_white(), 0);
	lv_label_set_text(soundLabel, "Sound");

	soundSwitch = lv_switch_create(sound);
	lv_obj_add_flag(soundSwitch, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
	lv_obj_clear_flag(soundSwitch, LV_OBJ_FLAG_CHECKABLE);

	lv_obj_add_event_cb(soundSwitch, [](lv_event_t* event){
		lv_obj_t* obj = static_cast<lv_obj_t*>(event->user_data);
		lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
		lv_obj_scroll_to_y(obj,0,LV_ANIM_ON);
		Buzz.setMuteEnter(true);
	}, LV_EVENT_FOCUSED, obj);

	lv_obj_add_event_cb(soundSwitch, [](lv_event_t* event){
		lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
		Buzz.setMuteEnter(false);
	}, LV_EVENT_DEFOCUSED, nullptr);

	lv_obj_add_event_cb(soundSwitch, [](lv_event_t* event){
		auto soundSwitch = static_cast<SettingsScreen*>(event->user_data);
		soundSwitch->pop();
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(soundSwitch, [](lv_event_t* event){
		auto sw = lv_event_get_target(event);
		Settings.get().sound = lv_obj_get_state(sw) & LV_STATE_CHECKED;
	}, LV_EVENT_VALUE_CHANGED, nullptr);

	//make the soundSwitch checkable ONLY when pressed with the ENTER key
	lv_obj_add_event_cb(soundSwitch, [](lv_event_t* event){
		auto sw = lv_event_get_target(event);
		lv_obj_add_flag(sw, LV_OBJ_FLAG_CHECKABLE);
		if(!(lv_obj_get_state(sw) & LV_STATE_CHECKED)){
			Piezo.setMute(false);
			Piezo.tone(NOTE_C5, 25);
		}else{
			Piezo.setMute(true);
		}
	}, LV_EVENT_PRESSED, nullptr);
	lv_obj_add_event_cb(soundSwitch, [](lv_event_t* event){
		lv_obj_clear_flag(lv_event_get_target(event), LV_OBJ_FLAG_CHECKABLE);
	}, LV_EVENT_RELEASED, nullptr);



	lv_group_add_obj(inputGroup, soundSwitch);

	lv_obj_remove_style(soundSwitch, nullptr,LV_STATE_FOCUS_KEY);
	lv_obj_set_style_border_width(soundSwitch, 1, LV_PART_INDICATOR | LV_STATE_CHECKED);
	lv_obj_set_style_border_color(soundSwitch, lv_color_white(), LV_PART_INDICATOR | LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(soundSwitch,lv_color_hex(0x813df5),LV_PART_INDICATOR | LV_STATE_CHECKED);
	lv_obj_set_style_bg_opa(soundSwitch, LV_OPA_100, LV_PART_INDICATOR | LV_STATE_CHECKED);
	lv_obj_set_style_border_width(soundSwitch, 1, 0);
	lv_obj_set_style_border_color(soundSwitch, lv_color_white(), 0);
	lv_obj_set_style_bg_opa(soundSwitch, LV_OPA_0, 0);
	lv_obj_set_size(soundSwitch, 25, 15);

	//sleepTime
	sleepTime = lv_obj_create(obj);
	lv_obj_set_height(sleepTime, LV_SIZE_CONTENT);
	lv_obj_set_width(sleepTime, lv_pct(100));
	lv_obj_set_layout(sleepTime, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(sleepTime, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(sleepTime, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_all(sleepTime, 3, 0);
	lv_obj_set_style_bg_opa(sleepTime, 0, 0);
	lv_obj_add_style(sleepTime, &style_pressed, selPress);
	lv_obj_add_style(sleepTime, &style_focused, selFocus);
	lv_obj_add_style(sleepTime, &style_def, sel);

	lv_obj_t* sleepLabel = lv_label_create(sleepTime);
	lv_obj_set_style_text_font(sleepLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(sleepLabel, lv_color_white(), 0);
	lv_obj_set_style_pad_top(sleepLabel, 2, 0);
	lv_label_set_text(sleepLabel, "Sleep time");

	sleepSlider = lv_slider_create(sleepTime);
	lv_obj_add_flag(sleepSlider, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	lv_obj_add_event_cb(sleepSlider, [](lv_event_t* event){
		lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_FOCUSED, nullptr);

	lv_obj_add_event_cb(sleepSlider, [](lv_event_t* event){
		lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_DEFOCUSED, nullptr);

	lv_obj_add_event_cb(sleepSlider, [](lv_event_t* event){
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		if(!(lv_obj_get_state(event->target) & LV_STATE_EDITED)){
			slider->pop();
		}
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(sleepSlider, [](lv_event_t* event){
		auto* settings = static_cast<SettingsScreen*>(event->user_data);

		lv_label_set_text(settings->sleepTimeLabel, SleepText[lv_slider_get_value(event->target)]);
	}, LV_EVENT_VALUE_CHANGED, this);

	lv_obj_add_event_cb(sleepSlider, [](lv_event_t* event){
		lv_obj_t* slider = static_cast<lv_obj_t*>(event->user_data);
		lv_group_t* group = static_cast<lv_group_t*>(lv_obj_get_group(slider));
		if(group == nullptr){
			return;
		}
		lv_event_send(slider,LV_EVENT_RELEASED, nullptr);
		lv_event_send(slider,LV_EVENT_SHORT_CLICKED, nullptr);
		lv_event_send(slider,LV_EVENT_CLICKED, nullptr);
		lv_group_send_data(group,LV_KEY_ENTER);
	}, LV_EVENT_CANCEL, sleepSlider);

	lv_group_add_obj(inputGroup, sleepSlider);

	lv_slider_set_range(sleepSlider, 0, SLEEP_STEPS - 1);
	lv_obj_remove_style_all(sleepSlider);        /*Remove the styles coming from the theme*/
	lv_obj_set_size(sleepSlider, 56, 12);
	lv_obj_set_style_pad_hor(sleepSlider, 5, 0);

	sleepTimeLabel = lv_label_create(sleepSlider);
	lv_obj_add_flag(sleepTimeLabel,LV_OBJ_FLAG_FLOATING);
	lv_obj_set_style_align(sleepTimeLabel,LV_ALIGN_CENTER,0);
	lv_obj_set_style_text_font(sleepTimeLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(sleepTimeLabel, lv_color_black(), 0);
	lv_obj_set_style_pad_top(sleepTimeLabel,1,0);
	lv_obj_set_style_text_color(sleepTimeLabel, lv_color_hex(0x892eff), 0);

	lv_obj_add_event_cb(sleepSlider, [](lv_event_t* event){
		if(lv_obj_get_state(event->target) & LV_STATE_EDITED){
			lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}else{
			lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}
	}, LV_EVENT_STYLE_CHANGED, nullptr);

	lv_style_init(&style_main);
	lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
	lv_style_set_bg_color(&style_main, lv_color_hex3(0xbbb));
	lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
	lv_obj_add_style(sleepSlider, &style_main, LV_PART_MAIN);

	lv_style_init(&style_knob);
	lv_style_set_bg_opa(&style_knob, LV_OPA_100);
	lv_obj_add_style(sleepSlider, &style_knob, LV_PART_KNOB);
	lv_style_set_text_font(&style_knob, &pixelbasic7);
	lv_style_set_text_color(&style_knob, lv_color_white());
	lv_style_set_height(&style_knob, 10);
	lv_style_set_width(&style_knob, 10);
	lv_obj_add_style(sleepSlider, &style_knob, LV_PART_KNOB | LV_STATE_EDITED);

	//shutdownTime
	shutdownTime = lv_obj_create(obj);
	lv_obj_set_height(shutdownTime, LV_SIZE_CONTENT);
	lv_obj_set_width(shutdownTime, lv_pct(100));
	lv_obj_set_layout(shutdownTime, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(shutdownTime, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(shutdownTime, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_all(shutdownTime, 3, 0);
	lv_obj_set_style_bg_opa(shutdownTime, 0, 0);
	lv_obj_add_style(shutdownTime, &style_pressed, selPress);
	lv_obj_add_style(shutdownTime, &style_focused, selFocus);
	lv_obj_add_style(shutdownTime, &style_def, sel);

	lv_obj_t* shutdownLabel = lv_label_create(shutdownTime);
	lv_obj_set_style_text_font(shutdownLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(shutdownLabel, lv_color_white(), 0);
	lv_obj_set_style_pad_top(shutdownLabel, 2, 0);
	lv_label_set_text(shutdownLabel, "Shutdown time");

	shutdownSlider = lv_slider_create(shutdownTime);
	lv_obj_add_flag(shutdownSlider, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_FOCUSED, nullptr);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_DEFOCUSED, nullptr);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		if(!(lv_obj_get_state(event->target) & LV_STATE_EDITED)){
			slider->pop();
		}
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		auto* settings = static_cast<SettingsScreen*>(event->user_data);

		lv_label_set_text(settings->shutdownTimeLabel, ShutdownText[lv_slider_get_value(event->target)]);
	}, LV_EVENT_VALUE_CHANGED, this);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		lv_obj_t* slider = static_cast<lv_obj_t*>(event->user_data);
		lv_group_t* group = static_cast<lv_group_t*>(lv_obj_get_group(slider));
		if(group == nullptr){
			return;
		}
		lv_event_send(slider,LV_EVENT_RELEASED, nullptr);
		lv_event_send(slider,LV_EVENT_SHORT_CLICKED, nullptr);
		lv_event_send(slider,LV_EVENT_CLICKED, nullptr);
		lv_group_send_data(group,LV_KEY_ENTER);
	}, LV_EVENT_CANCEL, shutdownSlider);

	lv_group_add_obj(inputGroup, shutdownSlider);

	lv_slider_set_range(shutdownSlider, 0, SHUTDOWN_STEPS - 1);
	lv_obj_remove_style_all(shutdownSlider);        /*Remove the styles coming from the theme*/
	lv_obj_set_size(shutdownSlider, 56, 12);
	lv_obj_set_style_pad_hor(shutdownSlider, 5, 0);

	shutdownTimeLabel = lv_label_create(shutdownSlider);
	lv_obj_add_flag(shutdownTimeLabel,LV_OBJ_FLAG_FLOATING);
	lv_obj_set_style_align(shutdownTimeLabel,LV_ALIGN_CENTER,0);
	lv_obj_set_style_text_font(shutdownTimeLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(shutdownTimeLabel, lv_color_black(), 0);
	lv_obj_set_style_pad_top(shutdownTimeLabel,1,0);
	lv_obj_set_style_text_color(shutdownTimeLabel, lv_color_hex(0x892eff), 0);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		if(lv_obj_get_state(event->target) & LV_STATE_EDITED){
			lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}else{
			lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}
	}, LV_EVENT_STYLE_CHANGED, nullptr);

	lv_style_init(&style_main);
	lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
	lv_style_set_bg_color(&style_main, lv_color_hex3(0xbbb));
	lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
	lv_obj_add_style(shutdownSlider, &style_main, LV_PART_MAIN);

	lv_style_init(&style_knob);
	lv_style_set_bg_opa(&style_knob, LV_OPA_100);
	lv_obj_add_style(shutdownSlider, &style_knob, LV_PART_KNOB);
	lv_style_set_text_font(&style_knob, &pixelbasic7);
	lv_style_set_text_color(&style_knob, lv_color_white());
	lv_style_set_height(&style_knob, 10);
	lv_style_set_width(&style_knob, 10);
	lv_obj_add_style(shutdownSlider, &style_knob, LV_PART_KNOB | LV_STATE_EDITED);

	//screenBrightness
	screenBrightness = lv_obj_create(obj);
	lv_obj_set_height(screenBrightness, LV_SIZE_CONTENT);
	lv_obj_set_width(screenBrightness, lv_pct(100));
	lv_obj_set_layout(screenBrightness, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(screenBrightness, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(screenBrightness, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_all(screenBrightness, 3, 0);
	lv_obj_set_style_bg_opa(screenBrightness, 0, 0);
	lv_obj_add_style(screenBrightness, &style_pressed, selPress);
	lv_obj_add_style(screenBrightness, &style_focused, selFocus);
	lv_obj_add_style(screenBrightness, &style_def, sel);

	lv_obj_t* brightnessLabel = lv_label_create(screenBrightness);
	lv_obj_set_style_text_font(brightnessLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(brightnessLabel, lv_color_white(), 0);
	lv_obj_set_style_pad_top(brightnessLabel, 2, 0);
	lv_label_set_text(brightnessLabel, "Brightness");

	brightnessSlider = lv_slider_create(screenBrightness);
	lv_obj_add_flag(brightnessSlider, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	setButtonHoldAndRepeatTime(BTN_LEFT, 50);
	setButtonHoldAndRepeatTime(BTN_RIGHT, 50);
	setButtonHoldTime(BTN_LEFT, 400);
	setButtonHoldTime(BTN_RIGHT, 400);

	lv_slider_set_range(brightnessSlider, 0, 51);
	lv_obj_remove_style_all(brightnessSlider);        /*Remove the styles coming from the theme*/
	lv_obj_set_size(brightnessSlider, 56, 12);
	lv_obj_set_style_pad_hor(brightnessSlider, 5, 0);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_FOCUSED, this);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_DEFOCUSED, this);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		if(!(lv_obj_get_state(event->target) & LV_STATE_EDITED)){
			slider->pop();
		}
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		if(lv_obj_get_state(event->target) & LV_STATE_EDITED){
			lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}else{
			lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}
	}, LV_EVENT_STYLE_CHANGED, nullptr);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		lv_obj_t* slider = static_cast<lv_obj_t*>(event->user_data);
		lv_group_t* group = static_cast<lv_group_t*>(lv_obj_get_group(slider));
		if(group == nullptr){
			return;
		}
		lv_event_send(slider,LV_EVENT_RELEASED, nullptr);
		lv_event_send(slider,LV_EVENT_SHORT_CLICKED, nullptr);
		lv_event_send(slider,LV_EVENT_CLICKED, nullptr);
		lv_group_send_data(group,LV_KEY_ENTER);
	}, LV_EVENT_CANCEL, brightnessSlider);

	lv_group_add_obj(inputGroup, brightnessSlider);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		lv_obj_t* slider = static_cast<lv_obj_t*>(event->user_data);
		Chatter.setBrightness(lv_slider_get_value(slider) * 5);
	}, LV_EVENT_VALUE_CHANGED, brightnessSlider);

	lv_style_init(&style_main);
	lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
	lv_style_set_bg_color(&style_main, lv_color_hex3(0xbbb));
	lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
	lv_obj_add_style(brightnessSlider, &style_main, LV_PART_MAIN);

	lv_style_init(&style_knob);
	lv_style_set_bg_opa(&style_knob, LV_OPA_100);
	lv_obj_add_style(brightnessSlider, &style_knob, LV_PART_KNOB);
	lv_style_set_text_font(&style_knob, &pixelbasic7);
	lv_style_set_text_color(&style_knob, lv_color_white());
	lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
	lv_style_set_height(&style_knob, 10);
	lv_style_set_width(&style_knob, 10);
	lv_obj_add_style(brightnessSlider, &style_knob, LV_PART_KNOB | LV_STATE_EDITED);

	//factoryReset
	factoryReset = lv_obj_create(obj);
	lv_obj_set_height(factoryReset, LV_SIZE_CONTENT);
	lv_obj_set_width(factoryReset, lv_pct(100));
	lv_obj_set_layout(factoryReset, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(factoryReset, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(factoryReset, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(factoryReset, 8, 0);
	lv_obj_set_style_pad_all(factoryReset, 3, 0);
	lv_obj_set_style_bg_opa(factoryReset, 0, 0);
	lv_obj_add_style(factoryReset, &style_focused, selFocus);
	lv_obj_add_style(factoryReset, &style_def, sel);
	lv_obj_add_flag(factoryReset, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	lv_obj_clear_flag(factoryReset, LV_OBJ_FLAG_CLICK_FOCUSABLE);
	lv_obj_clear_flag(factoryReset, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_clear_flag(factoryReset, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_add_event_cb(factoryReset, [](lv_event_t* event){
		auto hw = static_cast<SettingsScreen*>(event->user_data);
		auto prompt = new Prompt(hw, "Are you sure?\n\nThis will erase ALL data!");
		lv_obj_add_event_cb(prompt->getLvObj(), [](lv_event_t* e){
			Storage.Friends.clear();
			Storage.Convos.clear();
			Storage.Messages.clear();
			nvs_flash_erase();
			Settings.reset();
			Chatter.fadeOut();
			ledcDetachPin(PIN_BUZZ);
			ESP.restart();
		}, EV_PROMPT_YES, nullptr);
		prompt->start();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(factoryReset, [](lv_event_t* event){
		SettingsScreen* screen = static_cast<SettingsScreen*>(event->user_data);
		screen->pop();
	}, LV_EVENT_CANCEL, this);

	lv_group_add_obj(inputGroup, factoryReset);

	lv_obj_t* factoryResetLabel = lv_label_create(factoryReset);
	lv_obj_set_style_text_font(factoryResetLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(factoryResetLabel, lv_color_white(), 0);
	lv_label_set_text(factoryResetLabel, "Factory reset");

	//HWtest
	HWTest = lv_obj_create(obj);
	lv_obj_set_height(HWTest, LV_SIZE_CONTENT);
	lv_obj_set_width(HWTest, lv_pct(100));
	lv_obj_set_layout(HWTest, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(HWTest, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(HWTest, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(HWTest, 8, 0);
	lv_obj_set_style_pad_all(HWTest, 3, 0);
	lv_obj_set_style_bg_opa(HWTest, 0, 0);
	lv_obj_add_style(HWTest, &style_focused, selFocus);
	lv_obj_add_style(HWTest, &style_def, sel);
	lv_obj_add_flag(HWTest, LV_OBJ_FLAG_SCROLL_ON_FOCUS);


	lv_obj_t* HWTestLabel = lv_label_create(HWTest);
	lv_obj_set_style_text_font(HWTestLabel, &pixelbasic7, 0);
	lv_obj_set_style_text_color(HWTestLabel, lv_color_white(), 0);
	lv_label_set_text(HWTestLabel, "Hardware test");
	lv_obj_clear_flag(HWTest, LV_OBJ_FLAG_CLICK_FOCUSABLE);
	lv_obj_clear_flag(HWTest, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_clear_flag(HWTest, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_add_event_cb(HWTest, [](lv_event_t* event){
		auto* settings = static_cast<SettingsScreen*>(event->user_data);
		settings->push(new UserHWTest());
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(HWTest, [](lv_event_t* event){
		SettingsScreen* factory = static_cast<SettingsScreen*>(event->user_data);
		factory->pop();
	}, LV_EVENT_CANCEL, this);

	lv_group_add_obj(inputGroup, HWTest);
}

SettingsScreen::~SettingsScreen(){
	Input::getInstance()->removeListener(this);
	lv_style_reset(&style_knob);
	lv_style_reset(&style_def);
	lv_style_reset(&style_focused);
	lv_style_reset(&style_main);
	lv_style_reset(&style_pressed);
}

void SettingsScreen::onStarting(){
	if(Settings.get().sound){
		lv_obj_add_state(soundSwitch, LV_STATE_CHECKED);
	}
	lv_slider_set_value(sleepSlider, Settings.get().sleepTime, LV_ANIM_OFF);
	lv_slider_set_value(shutdownSlider, Settings.get().shutdownTime, LV_ANIM_OFF);
	lv_slider_set_value(brightnessSlider, Settings.get().screenBrightness / 5, LV_ANIM_OFF);

	lv_label_set_text(sleepTimeLabel, SleepText[Settings.get().sleepTime]);
	lv_label_set_text(shutdownTimeLabel, ShutdownText[Settings.get().shutdownTime]);

}

void SettingsScreen::onStart(){
	Input::getInstance()->addListener(this);
}

void SettingsScreen::onStop(){
	Settings.get().sound = lv_obj_get_state(soundSwitch) & LV_STATE_CHECKED;
	Settings.get().sleepTime = lv_slider_get_value(sleepSlider);
	Settings.get().shutdownTime = lv_slider_get_value(shutdownSlider);
	Settings.get().screenBrightness = lv_slider_get_value(brightnessSlider) * 5;
	Settings.store();
	Sleep.updateTimes();
	Input::getInstance()->removeListener(this);
}

void SettingsScreen::buttonHeldRepeat(uint i, uint repeatCount){
	if(!lv_obj_has_state(brightnessSlider, LV_STATE_EDITED) || !heldThresh) return;

	if(i == BTN_LEFT && lv_slider_get_value(brightnessSlider) > 0){
		lv_slider_set_value(brightnessSlider, lv_slider_get_value(brightnessSlider) - 1, LV_ANIM_ON);
	}else if(i == BTN_RIGHT && lv_slider_get_value(brightnessSlider) < 51){
		lv_slider_set_value(brightnessSlider, lv_slider_get_value(brightnessSlider) + 1, LV_ANIM_ON);
	}
	Chatter.setBrightness(lv_slider_get_value(brightnessSlider) * 5);
}

void SettingsScreen::buttonReleased(uint i){
	if(!lv_obj_has_state(brightnessSlider, LV_STATE_EDITED) && heldThresh) return;

	if(i == BTN_LEFT || i == BTN_RIGHT){
		heldThresh = false;
	}
}

void SettingsScreen::buttonHeld(uint i){
	if(!lv_obj_has_state(brightnessSlider, LV_STATE_EDITED) && !heldThresh) return;

	if(i == BTN_LEFT || i == BTN_RIGHT){
		heldThresh = true;
	}
}

void SettingsScreen::buttonPressed(uint i){
	if(lv_obj_has_state(brightnessSlider, LV_STATE_EDITED)
		|| lv_obj_has_state(sleepSlider, LV_STATE_EDITED)
		|| lv_obj_has_state(shutdownSlider, LV_STATE_EDITED)) return;

	if(i == BTN_R){
		LockScreen::activate(this);
	}
}
