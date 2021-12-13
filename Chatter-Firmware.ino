#include <Arduino.h>
#include <CircuitOS.h>
#include <lvgl.h>
#include "src/InputChatter.h"
#include "src/FSLVGL.h"
//#include <Chatter.h>
#include "src/LVScreen.h"
#include "src/User.h"
#include <Loop/LoopManager.h>
#include <ByteBoi.h>
#include <SPIFFS.h>


#define TFT_WIDTH 160
#define TFT_HEIGHT 120
lv_disp_draw_buf_t drawBuffer;
lv_color_t buf[TFT_WIDTH * TFT_HEIGHT];
Display* display;
Sprite* canvas;




void my_print(const char* c){
	Serial.println(c);
	Serial.flush();
}

void lvglFlush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p){
	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);

	canvas->drawIcon(&color_p->full, area->x1, area->y1, w, h);
	display->commit();

	/*
	TFT_eSPI tft = *display->getTft();
 	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors(&color_p->full, w * h, true);
	tft.endWrite();
	 */

	lv_disp_flush_ready(disp);
}

class TestScreen : public LVScreen{
public:
	TestScreen() : LVScreen(){
		lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
		lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_AUTO);
		lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);


		lv_obj_t* img = lv_img_create(obj);
		lv_img_set_src(img, "S:/test.bin");
		lv_obj_set_style_border_width(img, 2, LV_STATE_DEFAULT);
		lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
		lv_obj_set_size(img, 160, 128);
		Serial.println("----------------");

/*		for(int i = 0; i < 5; i++){
			User* user = new User(this, esp_random() % 360, "Foo " + String(i + 1));
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
/*	Chatter.begin();

	display = Chatter.getDisplay();*/

	ByteBoi.begin();
	Battery.disableShutdown(true);
	ByteBoi.unbindMenu();

	display = ByteBoi.getDisplay();
	canvas = display->getBaseSprite();

	lv_init();
	lv_disp_draw_buf_init(&drawBuffer, buf, NULL, TFT_WIDTH * TFT_HEIGHT);
//	lv_log_register_print_cb(my_print); /* register print function for debugging */

	static lv_disp_drv_t displayDriver;
	lv_disp_drv_init(&displayDriver);
	/*Change the following line to your display resolution*/
	displayDriver.hor_res = TFT_WIDTH;
	displayDriver.ver_res = TFT_HEIGHT;
	displayDriver.flush_cb = lvglFlush;
	displayDriver.draw_buf = &drawBuffer;
	lv_disp_drv_register(&displayDriver);

	new FSLVGL(SPIFFS, 'S');
	ByteBoi.getInput()->addListener(new InputChatter());
//	Chatter.getInput()->addListener(new InputChatter());

	TestScreen* screen = new TestScreen();
	screen->start();
}

void loop(){
	lv_timer_handler();
	LoopManager::loop();
}

