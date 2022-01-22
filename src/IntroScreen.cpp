#include "IntroScreen.h"
#include "MainMenu.h"

IntroScreen::IntroScreen(){
	gif = lv_gif_create(obj);
	lv_gif_set_src(gif, "S:/intro.gif");
	lv_gif_set_loop(gif, LV_GIF_LOOP_SINGLE);

	lv_obj_add_event_cb(gif, [](lv_event_t * e){
		IntroScreen* intro = static_cast<IntroScreen*>(e->user_data);
		intro->stop();
		delete intro;

		MainMenu* menu = new MainMenu();
		menu->start();
	}, LV_EVENT_READY, this);

	lv_gif_start(gif);
}

void IntroScreen::onStop(){
	lv_gif_stop(gif);
	lv_obj_del(gif);
}
