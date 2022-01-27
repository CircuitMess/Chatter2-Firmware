#include "TextEntry.h"
#include <Input/Input.h>
#include <Pins.hpp>
#include <Loop/LoopManager.h>

const char* TextEntry::characters[] = {
		"1.!?",
		"abc2",
		"def3",
		"ghi4",
		"jkl5",
		"mno6",
		"pqrs7",
		"tuv8",
		"wxyz9",
		" 0"
};

const std::map<uint8_t, uint8_t> TextEntry::keyMap = {
		{ BTN_1, 0 },
		{ BTN_2, 1 },
		{ BTN_3, 2 },
		{ BTN_4, 3 },
		{ BTN_5, 4 },
		{ BTN_6, 5 },
		{ BTN_7, 6 },
		{ BTN_8, 7 },
		{ BTN_9, 8 },
		{ BTN_0, 9 },
};

TextEntry::TextEntry(lv_obj_t* parent, const std::string& text) : LVObject(parent), text(text){
	lv_obj_set_size(obj, lv_pct(100), LV_SIZE_CONTENT);

	// Focused style
	entry = lv_textarea_create(obj);
	lv_obj_set_width(entry, lv_pct(100));
	lv_textarea_set_one_line(entry, true);
	lv_textarea_set_text(entry, text.c_str());

	lv_style_init(&entryFocus);
	lv_obj_add_style(entry, &entryFocus, LV_PART_CURSOR | LV_STATE_FOCUSED);
}

void TextEntry::setText(const std::string& text){
	this->text = text;
	lv_textarea_set_text(entry, text.c_str());
	lv_obj_invalidate(entry);
}

void TextEntry::setTextColor(lv_color_t color){
	lv_obj_set_style_text_color(entry, color, LV_PART_MAIN | LV_STATE_ANY);
}

void TextEntry::setPlaceholder(const std::string& text){
	lv_textarea_set_placeholder_text(entry, text.c_str());
}

bool TextEntry::isActive() const{
	return active;
}

const std::string& TextEntry::getText() const{
	return text;
}

void TextEntry::clear(){
	setText("");
}

void TextEntry::start(){
	Input::getInstance()->addListener(this);
	lv_obj_add_state(entry, LV_STATE_FOCUSED);
	active = true;
}

void TextEntry::stop(){
	Input::getInstance()->removeListener(this);
	lv_obj_clear_state(entry, LV_STATE_FOCUSED);
	active = false;
}

void TextEntry::buttonPressed(uint i){
	if(!active){
		Input::getInstance()->removeListener(this);
		return;
	}

	if(i == BTN_ENTER){
		stop();
		lv_event_send(obj, EV_ENTRY_DONE, nullptr);
		return;
	}

	if(i == BTN_BACK){
		stop();
		lv_event_send(obj, EV_ENTRY_CANCEL, nullptr);
		return;
	}

	if(i == BTN_LEFT || i == BTN_RIGHT) return;

	keyPress(i);
}

void TextEntry::keyPress(uint8_t i){
	if(i == BTN_L){
		if(text.empty()) return;
		text = text.substr(0, text.size() - 1);
		setText(text);
		return;
	}

	if(!keyMap.count(i)) return;
	uint8_t key = keyMap.at(i);
	const char* chars = characters[key];

	if(key == currentKey && keyTime != 0){
		index = (index + 1) % strnlen(chars, 10);
		text.back() = chars[index];
	}else{
		currentKey = key;
		index = 0;
		text.append(1, chars[index]);
	}

	if(keyTime == 0){
		LoopManager::addListener(this);
	}

	keyTime = millis();

	//lv_obj_set_style_anim_time(entry, 0, LV_PART_CURSOR | LV_STATE_FOCUSED);
	setText(text);
}

void TextEntry::loop(uint micros){
	if(millis() - keyTime < 500) return;

	keyTime = 0;
	lv_obj_set_style_anim_time(entry, 500, LV_PART_CURSOR | LV_STATE_FOCUSED);
	LoopManager::removeListener(this);
}
