#include "IntroScreen.h"
#include "../Screens/MainMenu.h"

IntroScreen::IntroScreen(){
	splash = lv_img_create(obj);
	lv_img_set_src(splash, "S:/splash.bin");
}

void IntroScreen::onStop(){
	lv_gif_stop(gif);
	lv_obj_del(gif);
}

void IntroScreen::startAnim(void (* callback)()){
	lv_obj_del(splash);
	IntroScreen::callback = callback;
	gif = lv_gif_create(obj);
	lv_gif_set_src(gif, "S:/intro.gif");
	lv_gif_set_loop(gif, LV_GIF_LOOP_SINGLE);

	lv_obj_add_event_cb(gif, [](lv_event_t * e){
		IntroScreen* intro = static_cast<IntroScreen*>(e->user_data);
		intro->stop();
		volatile auto temp  = intro->callback;
		delete intro;

		MainMenu* menu = new MainMenu();
		menu->start();
		if(temp != nullptr) temp();
	}, LV_EVENT_READY, this);

	lv_gif_start(gif);
}
