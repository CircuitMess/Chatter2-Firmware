#include <Arduino.h>
#include <CircuitOS.h>
#include <lvgl.h>
#include "src/InputChatter.h"
#include "src/FSLVGL.h"
#include <Chatter.h>
#include "src/LVScreen.h"
#include "src/UserWithMessage.h"
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
//#include "src/EditableAvatar.h"
#include "src/Inbox.h"
#include "src/ChatterTheme.h"
#include "src/EmojiMenu.h"

lv_disp_draw_buf_t drawBuffer;
Display* display;
std::vector<Profile> friends = { Profile{ "Mauricije", 0, 40}, Profile{ "Nikola", 1, 100}, Profile{ "MMOMOMOMMMMM", 2, 160},
								 Profile{ "Mauricije", 0, 40}, Profile{ "Nikola", 1, 100}, Profile{ "MMOMOMOMMMMM", 2, 160}};


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
Profile profile{ "Mauricije", 0, 40};
class TestScreen : public LVScreen {
public:
	TestScreen() : LVScreen(){
/*		lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
		lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ON);
		lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
		lv_obj_set_style_pad_gap(obj, 0, 0);*/
		lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
		lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
//		lv_obj_set_style_pad_all(obj, 10, 0);
//		lv_group_add_obj(inputGroup, (new EditableAvatar(obj))->getLvObj());
		emMenu = new EmojiMenu(obj, [](uint8_t id, void* userData){
			static_cast<TestScreen*>(userData)->emojiResult(id);
		}, this);
		lv_obj_add_flag(emMenu->getLvObj(), LV_OBJ_FLAG_HIDDEN);
		lv_obj_set_align(emMenu->getLvObj(), LV_ALIGN_CENTER);

/*		lv_obj_t* img = lv_img_create(obj);
		lv_img_set_src(img, "S:/test.bin");
		lv_obj_set_style_border_width(img, 2, LV_STATE_DEFAULT);
		lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
		lv_obj_set_size(img, 160, 128);
		Serial.println("----------------");*/
/*		lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
			lv_obj_t* focused = lv_group_get_focused(group);
			lv_obj_scroll_to_view(focused, LV_ANIM_ON);
		});

		for(int i = 0; i < 5; i++){
//			User* user = new UserWithMessage(obj, profile, "Lorem");
			auto user = new UserWithMessage(obj, profile, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
//			User* user = new User(obj, profile);



			lv_group_add_obj(inputGroup, user->getLvObj());
		}*/
	};
	void enter(){
		emMenu->enter(inputGroup);
		lv_obj_clear_flag(emMenu->getLvObj(), LV_OBJ_FLAG_HIDDEN);
	};
	void emojiResult(uint8_t result){
		Serial.printf("emoji: %d\n", result);
		lv_obj_add_flag(emMenu->getLvObj(), LV_OBJ_FLAG_HIDDEN);
	};
	EmojiMenu* emMenu;
};

TestScreen* screen;

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
	lv_disp_t * disp = lv_disp_drv_register(&displayDriver);
	chatterThemeInit(disp);

	new FSLVGL(SPIFFS, 'S');

	Chatter.getInput()->addListener(new InputChatter());

	screen = new TestScreen();
	screen->start();
}
bool once = true;
void loop(){
	if(once && millis() > 3000){
		once = false;
		screen->enter();
	}
	lv_timer_handler();
	LoopManager::loop();
}

