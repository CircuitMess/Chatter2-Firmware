#ifndef CHATTER_FIRMWARE_TEXTENTRY_H
#define CHATTER_FIRMWARE_TEXTENTRY_H

#include <Arduino.h>
#include <lvgl.h>
#include "../Interface/LVObject.h"
#include <string>
#include <Input/InputListener.h>
#include <Loop/LoopListener.h>

#define EV_ENTRY_DONE ((lv_event_code_t) (_LV_EVENT_LAST + 1))
#define EV_ENTRY_CANCEL ((lv_event_code_t) (_LV_EVENT_LAST + 2))
#define EV_ENTRY_LR ((lv_event_code_t) (_LV_EVENT_LAST + 3))

class TextEntry : public LVObject, private InputListener, public LoopListener {
public:
	TextEntry(lv_obj_t* parent, const std::string& text = "", uint32_t maxLength = -1);
	virtual ~TextEntry();

	void setTextColor(lv_color_t color);
	void setPlaceholder(const std::string& text);
	void setText(const std::string& text);
	std::string getText() const;
	void showCaps(bool show);

	void keyPress(uint8_t i);
	void clear();

	void start();
	void stop();
	void focus();
	void defocus();
	bool isActive() const;

	void loop(uint micros) override;

private:
	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;
	void buttonHeld(uint i) override;
	void buttonHeldRepeat(uint i, uint repeatCount) override;

	void backspace();

	static const char* characters[];
	static char* charMap;
	static const std::map<uint8_t, uint8_t> keyMap;

	lv_obj_t* entry;
	lv_obj_t* capsText;
	bool active = false;

	lv_group_t* activeGroup = nullptr;
	lv_group_t* inputGroup;

	int8_t currentKey = -1; // currently active key
	uint8_t index = 0; // character under the key
	uint32_t keyTime = 0; // when the key was last pressed

	bool btnRHeld = false;

	enum CapsMode {
		LOWER, SINGLE, UPPER, COUNT
	} capsMode = LOWER;
	void setCapsMode(CapsMode mode);
};

#endif //CHATTER_FIRMWARE_TEXTENTRY_H
