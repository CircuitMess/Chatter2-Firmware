#include <Arduino.h>
#include <CircuitOS.h>
#include <lvgl.h>
#include "src/InputChatter.h"
//#include <Chatter.h>
#include "src/LVScreen.h"
#include "src/User.h"
#include <Loop/LoopManager.h>
#include <ByteBoi.h>
#include "src/ChatterTheme.h"

#define TFT_WIDTH 160
#define TFT_HEIGHT 120
lv_disp_draw_buf_t drawBuffer;
lv_color_t buf[TFT_WIDTH * TFT_HEIGHT];
Display* display;
Sprite* canvas;

void lvglPrint(lv_log_level_t level, const char* file, uint32_t line, const char* dsc){

	Serial.printf("%s@%d->%s\r\n", file, line, dsc);
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
		lv_obj_set_layout(screen, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
		lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_AUTO);
		lv_obj_set_style_pad_row(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);


		for(int i = 0; i < 5; i++){
			User* user = new User(screen, esp_random() % 360, "Foo " + String(i + 1));
			lv_group_add_obj(inputGroup, user->getLvObj());
		}

		lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
			lv_obj_t* focused = lv_group_get_focused(group);
			lv_obj_scroll_to(focused->parent, 0, focused->coords.y1, LV_ANIM_ON);
		});
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

	static lv_disp_drv_t displayDriver;
	lv_disp_drv_init(&displayDriver);
	/*Change the following line to your display resolution*/
	displayDriver.hor_res = TFT_WIDTH;
	displayDriver.ver_res = TFT_HEIGHT;
	displayDriver.flush_cb = lvglFlush;
	displayDriver.draw_buf = &drawBuffer;
	lv_disp_t * disp = lv_disp_drv_register(&displayDriver);
	chatterThemeInit(disp);

	ByteBoi.getInput()->addListener(new InputChatter());
//	Chatter.getInput()->addListener(new InputChatter());

	TestScreen* screen = new TestScreen();
	screen->start();
}

void loop(){
	lv_timer_handler();
	LoopManager::loop();
}
