#ifndef CHATTER_FIRMWARE_USERHWTEST_H
#define CHATTER_FIRMWARE_USERHWTEST_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include <unordered_map>
#include <Loop/LoopListener.h>
#include "../Interface/LVScreen.h"

class UserHWTest : public LVScreen, private InputListener, public LoopListener {
public:
	UserHWTest(void (*doneCallback)() = nullptr);
	~UserHWTest() override;

	void onStart() override;
	void onStop() override;
	void loop(uint micros) override;

private:
	lv_obj_t* canvas;
	static lv_color_t* canvasBuffer;
	static bool usingCanvas;
	std::vector<lv_obj_t*> labels;

	std::array<bool, 16> buttons;
	uint8_t buttonCount = 0;
	struct KeyCoord { uint8_t x, y; };
	static const std::unordered_map<uint8_t, KeyCoord> KeyCoords;

	uint32_t noteTime;
	uint8_t noteIndex;
	struct Note { uint16_t freq, duration; };
	static const std::vector<Note> Notes;

	uint8_t stage = 0;
	void (*doneCallback)() = nullptr;

	void buttonPressed(uint i) override;
	void startBuzz();

};


#endif //CHATTER_FIRMWARE_USERHWTEST_H
