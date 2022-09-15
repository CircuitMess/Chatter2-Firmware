#include "UnlockSlide.h"
#include <utility>
#include <Loop/LoopManager.h>
#include <font/lv_font.h>

UnlockSlide::UnlockSlide(lv_obj_t* parent, std::function<void()> onDone) : LVObject(parent), onDone(std::move(onDone)){
	lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
	lv_obj_set_style_pad_hor(obj, 4, 0);
	lv_obj_set_style_pad_ver(obj, 3, 0);

	text = lv_label_create(obj);
	lv_label_set_text(text, "Hold    to unlock");
	lv_obj_set_style_text_font(text, &lv_font_unscii_8, 0);
	lv_obj_set_style_text_color(text, lv_color_white(), 0);
	lv_obj_set_align(text, LV_ALIGN_BOTTOM_RIGHT);
	lv_obj_set_x(text, -2);

	icon = lv_img_create(obj);
	lv_img_set_src(icon, "S:/Lock/Icon.bin");
	lv_obj_set_pos(icon, 54, 5);

	lock = lv_img_create(obj);
	lv_img_set_src(lock, "S:/Lock/Locked.bin");
	lv_obj_set_align(lock, LV_ALIGN_BOTTOM_LEFT);
}

void UnlockSlide::start(){
	LoopManager::addListener(this);
	lv_img_set_src(lock, "S:/Lock/Unlocked.bin");
	state = Slide;
	t = 0;
}

void UnlockSlide::stop(){
	LoopManager::removeListener(this);
	state = Idle;
}

void UnlockSlide::shake(){
	LoopManager::addListener(this);
	state = Shake;
	t = 0;
}

void UnlockSlide::reset(){
	t = 0;
	repos();
	lv_img_set_src(lock, "S:/Lock/Locked.bin");
	state = Idle;
}

void UnlockSlide::loop(uint32_t dt){
	float speed = 1.0f;
	if(state == Slide){
		speed = slideSpeed;
	}else if(state == Shake){
		speed = shakeSpeed;
	}

	t = std::min(1.0f, t + speed * (float) dt / 1000000.0f);

	repos();

	if(t == 1.0f){
		LoopManager::removeListener(this);

		auto oldState = state;
		state = Idle;

		if(oldState == Slide){
			onDone();
		}
	}
}

void UnlockSlide::repos(){
	if(state == Shake){
		float t = this->t * 4;

		float x;
		static constexpr float fullDist = shakeHalfDist * 2;
		if(t < .5f){
			float t2 = t * 2.0f;
			x = shakeHalfDist * t2;
		}else if(t < 1.5f){
			float t2 = t - .5f;
			x = shakeHalfDist - fullDist * t2;
		}else if(t < 2.5f){
			float t2 = t - 1.5f;
			x = -shakeHalfDist + fullDist * t2;
		}else if(t < 3.5f){
			float t2 = t - 2.5f;
			x = shakeHalfDist - fullDist * t2;
		}else if(t < 4.f){
			float t2 = (t - 3.5f) * 2.f;
			x = -shakeHalfDist + shakeHalfDist * t2;
		}else{
			x = 0;
		}

		lv_obj_set_x(lock, std::round(x));
	}else{
		lv_obj_set_x(lock, std::round(t * (float) (lv_obj_get_width(obj) - 4)));
	}
}
