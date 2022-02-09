#include "TextEntry.h"
#include "InputLVGL.h"
#include <Input/Input.h>
#include <Pins.hpp>
#include <Loop/LoopManager.h>

const char* TextEntry::characters[] = {
		".,?!-:()*1",
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

TextEntry::TextEntry(lv_obj_t* parent, const std::string& text, uint32_t maxLength) : LVObject(parent), text(text){
	lv_obj_set_size(obj, lv_pct(100), LV_SIZE_CONTENT);

	// Focused style
	entry = lv_textarea_create(obj);
	lv_obj_clear_flag(entry, LV_OBJ_FLAG_CLICK_FOCUSABLE);
	lv_obj_clear_flag(entry, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_clear_flag(entry, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_width(entry, lv_pct(100));
	lv_textarea_set_one_line(entry, true);
	lv_textarea_set_text(entry, text.c_str());
	lv_textarea_set_max_length(entry, maxLength);

	lv_obj_set_style_border_width(entry, 1, 0);
	lv_obj_set_style_border_opa(entry, LV_OPA_0, 0);

	lv_style_init(&entryFocus);
	lv_obj_add_style(entry, &entryFocus, LV_PART_CURSOR | LV_STATE_FOCUSED);

	inputGroup = lv_group_create();
	lv_group_add_obj(inputGroup, entry);
	lv_obj_clear_state(entry, LV_STATE_FOCUSED);
}

TextEntry::~TextEntry(){
	lv_group_del(inputGroup);
}

void TextEntry::setText(const std::string& text){
	this->text = text;
	lv_textarea_set_text(entry, text.c_str());
	lv_obj_invalidate(entry);
}

void TextEntry::setTextColor(lv_color_t color){
	lv_obj_set_style_text_color(entry, color, LV_PART_MAIN | LV_STATE_DEFAULT);
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
	for(auto pair : keyMap){
		setButtonHoldTime(pair.first, 500);
	}
	setButtonHoldTime(BTN_R, 500);
	setButtonHoldAndRepeatTime(BTN_L, 250);

	lv_obj_add_state(obj, LV_STATE_EDITED);
	active = true;

	activeGroup = InputLVGL::getInstance()->getIndev()->group;
	lv_group_set_editing(activeGroup, true);
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), inputGroup);
	focus();

	lv_obj_add_event_cb(entry, [](lv_event_t* e){
		auto* entry = static_cast<TextEntry*>(e->user_data);
		if(!entry->active) return;

		entry->stop();
		lv_event_send(entry->obj, EV_ENTRY_DONE, nullptr);
	}, LV_EVENT_PRESSED, this);

	lv_obj_add_event_cb(entry, [](lv_event_t* e){
		auto* entry = static_cast<TextEntry*>(e->user_data);
		if(!entry->active) return;

		entry->stop();
		lv_event_send(entry->obj, EV_ENTRY_CANCEL, nullptr);
	}, LV_EVENT_CANCEL, this);
}

void TextEntry::stop(){
	lv_obj_clear_state(obj, LV_STATE_EDITED);
	lv_obj_remove_event_cb_with_user_data(entry, nullptr, this);

	if(activeGroup != nullptr){
		lv_indev_set_group(InputLVGL::getInstance()->getIndev(), activeGroup);
		lv_group_set_editing(activeGroup, false);
		activeGroup = nullptr;
	}
	defocus();

	Input::getInstance()->removeListener(this);
	active = false;
}

void TextEntry::focus(){
	lv_group_focus_obj(entry);
}

void TextEntry::defocus(){
	lv_obj_clear_state(entry, LV_STATE_FOCUSED);
}

void TextEntry::backspace(){
	if(text.empty()) return;

	text = text.substr(0, text.size() - 1);
	lv_textarea_del_char(entry);

	if(text.empty()){
		lv_event_send(entry, LV_EVENT_CANCEL, nullptr);
	}
}

void TextEntry::keyPress(uint8_t i){
	if(text.size() == lv_textarea_get_max_length(entry)) return;

	if(!keyMap.count(i)) return;
	uint8_t key = keyMap.at(i);
	const char* chars = characters[key];

	if(key == currentKey && keyTime != 0){
		index = (index + 1) % strnlen(chars, 10);
		text.back() = chars[index];

		lv_textarea_del_char(entry);
		lv_textarea_add_char(entry, chars[index]);
		lv_obj_scroll_to_x(entry, LV_COORD_MAX, LV_ANIM_OFF);
	}else{
		currentKey = key;
		index = 0;
		text.append(1, chars[index]);

		lv_textarea_add_char(entry, chars[index]);
	}

	if(keyTime == 0){
		LoopManager::addListener(this);
	}

	keyTime = millis();
}

void TextEntry::buttonPressed(uint i){
	if(i == BTN_LEFT || i == BTN_RIGHT){
		// TODO: cursor pos
		return;
	}

	if(i == BTN_ENTER || i == BTN_BACK) return;

	if(i == BTN_L){
		backspace();
		return;
	}

	if(i == BTN_R) return;

	keyPress(i);
}

void TextEntry::buttonHeldRepeat(uint i, uint repeatCount){
	if(i != BTN_L) return;

	backspace();
}

void TextEntry::buttonHeld(uint i){
	if(i == BTN_R){
		btnRHeld = true;
		// TODO: open meme menu
		return;
	}

	if(!keyMap.count(i)) return;
	uint8_t key = keyMap.at(i);

	if(key != currentKey) return;

	const char* chars = characters[key];
	char last = chars[strnlen(chars, 10) - 1];

	lv_textarea_del_char(entry);
	lv_textarea_add_char(entry, last);
	lv_obj_scroll_to_x(entry, LV_COORD_MAX, LV_ANIM_OFF);

	keyTime = 0;
	LoopManager::removeListener(this);
}

void TextEntry::buttonReleased(uint i){
	if(i != BTN_R) return;

	if(btnRHeld){
		btnRHeld = false;
		return;
	}

	// TODO: set caps mode
}

void TextEntry::loop(uint micros){
	if(millis() - keyTime < 500) return;

	keyTime = 0;
	lv_obj_set_style_anim_time(entry, 500, LV_PART_CURSOR | LV_STATE_FOCUSED);
	LoopManager::removeListener(this);
}
