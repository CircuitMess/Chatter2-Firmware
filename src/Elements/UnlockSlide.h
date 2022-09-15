#ifndef CHATTER2_FIRMWARE_UNLOCKSLIDE_H
#define CHATTER2_FIRMWARE_UNLOCKSLIDE_H

#include <Arduino.h>
#include <Loop/LoopListener.h>
#include "../Interface/LVObject.h"
#include <functional>

class UnlockSlide : public LVObject, private LoopListener {
public:
	UnlockSlide(lv_obj_t* parent, std::function<void()> onDone);

	void start();
	void stop();
	void reset();
	void shake();

private:
	void loop(uint32_t dt);

	enum State {
		Idle, Shake, Slide
	} state = Idle;

	lv_obj_t* text;
	lv_obj_t* icon;
	lv_obj_t* lock;

	std::function<void()> onDone;

	float t = 0;
	void repos();

	static constexpr float slideSpeed = 1.6f;
	static constexpr float shakeSpeed = 2.6f;
	static constexpr float shakeHalfDist = 2.0f;

};


#endif //CHATTER2_FIRMWARE_UNLOCKSLIDE_H
