#include <Arduino.h>
#include <CircuitOS.h>
#include <lvgl.h>
#include "src/InputChatter.h"
#include "src/FSLVGL.h"
#include <Chatter.h>
#include "src/LVScreen.h"
#include "src/User.h"
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
#include "src/EditableAvatar.h"

lv_disp_draw_buf_t drawBuffer;
Display* display;


void my_print(const char* c){
	Serial.println(c);
	Serial.flush();
}

void lvglFlush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p){
	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);

	TFT_eSPI &tft = *display->getTft();
	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors(&color_p->full, w * h, true);
	tft.endWrite();
	lv_disp_flush_ready(disp);
}

class TestScreen : public LVScreen {
public:
	TestScreen() : LVScreen(){
		lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
		lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_AUTO);
		lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
		lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);

		lv_group_add_obj(inputGroup, (new EditableAvatar(obj, 0, true))->getLvObj());

		lv_group_add_obj(inputGroup, (new EditableAvatar(obj, 0, true))->getLvObj());

/*		for(int i = 0; i < 5; i++){
			User* user = new User(obj, esp_random() % 360, "Foo " + String(i + 1));
			lv_group_add_obj(inputGroup, user->getLvObj());
		}

		lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
			lv_obj_t* focused = lv_group_get_focused(group);
			lv_obj_scroll_to(focused->parent, 0, focused->coords.y1, LV_ANIM_ON);
		});*/
	};
};


void setup(){
	Serial.begin(115200);
	Chatter.begin();
	display = Chatter.getDisplay();
	lv_init();
	lv_disp_draw_buf_init(&drawBuffer, display->getBaseSprite()->getBuffer(), NULL, 160 * 128);
//	lv_log_register_print_cb(my_print); /* register print function for debugging */

	static lv_disp_drv_t displayDriver;
	lv_disp_drv_init(&displayDriver);
	/*Change the following line to your display resolution*/
	displayDriver.hor_res = 160;
	displayDriver.ver_res = 128;
	displayDriver.flush_cb = lvglFlush;
	displayDriver.draw_buf = &drawBuffer;
	lv_disp_drv_register(&displayDriver);


	new FSLVGL(SPIFFS, 'S');

	Chatter.getInput()->addListener(new InputChatter());

	TestScreen* screen = new TestScreen();
	screen->start();
}

void loop(){
	lv_timer_handler();
	LoopManager::loop();
}

