#include "TextEntry.h"
#include "../InputLVGL.h"
#include "../Fonts/font.h"
#include <Input/Input.h>
#include <Pins.hpp>
#include <Loop/LoopManager.h>

const char* TextEntry::characters[] = {
		".,?!+-:()*1",
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

char* TextEntry::charMap = nullptr;

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

TextEntry::TextEntry(lv_obj_t* parent, const std::string& text, uint32_t maxLength) : LVObject(parent){
	lv_obj_set_size(obj, lv_pct(100), LV_SIZE_CONTENT);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);

	entry = lv_textarea_create(obj);
	lv_obj_clear_flag(entry, LV_OBJ_FLAG_CLICK_FOCUSABLE);
	lv_obj_clear_flag(entry, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_clear_flag(entry, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_flex_grow(entry, 1);
	lv_textarea_set_one_line(entry, true);
	lv_textarea_set_text(entry, text.c_str());
	lv_textarea_set_max_length(entry, maxLength);

	lv_obj_set_style_border_width(entry, 1, 0);
	lv_obj_set_style_border_opa(entry, LV_OPA_0, 0);

	if(charMap == nullptr){
		std::unordered_set<char> set;

		for(auto chars : characters){
			size_t n = strnlen(chars, 15);

			for(int i = 0; i < n; i++){
				set.insert(toLowerCase(chars[i]));
				set.insert(toUpperCase(chars[i]));
			}
		}

		charMap = static_cast<char*>(malloc(set.size() + 1));
		size_t i = 0;
		for(char c : set){
			charMap[i++] = c;
		}
		charMap[i] = 0;
	}

	lv_textarea_set_accepted_chars(entry, charMap);

	capsText = lv_label_create(obj);
	lv_obj_set_width(capsText, 14);
	lv_obj_set_style_pad_bottom(capsText, 2, 0);
	lv_obj_set_style_text_align(capsText, LV_TEXT_ALIGN_RIGHT, 0);
	lv_obj_set_style_text_font(capsText, &pixelbasic7, 0);
	lv_obj_set_style_text_color(capsText, lv_color_hex(0x8e478c), 0);
	lv_obj_add_flag(capsText, LV_OBJ_FLAG_HIDDEN);

	inputGroup = lv_group_create();
	lv_group_add_obj(inputGroup, entry);
	lv_obj_clear_state(entry, LV_STATE_FOCUSED);

	setCapsMode(LOWER);

	for(auto pair : keyMap){
		setButtonHoldTime(pair.first, 500);
	}
	setButtonHoldTime(BTN_R, 500);
	setButtonHoldAndRepeatTime(BTN_L, 250);
	setButtonHoldAndRepeatTime(BTN_LEFT, 250);
	setButtonHoldAndRepeatTime(BTN_RIGHT, 250);
}

TextEntry::~TextEntry(){
	lv_group_del(inputGroup);
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void TextEntry::setText(const std::string& text){
	lv_textarea_set_text(entry, text.c_str());
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

std::string TextEntry::getText() const{
	return lv_textarea_get_text(entry);
}

void TextEntry::clear(){
	setText("");
}

void TextEntry::start(){
	Input::getInstance()->addListener(this);

	btnRHeld = false;

	lv_obj_add_state(obj, LV_STATE_EDITED);
	active = true;

	activeGroup = InputLVGL::getInstance()->getIndev()->group;
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
		activeGroup = nullptr;
	}
	defocus();

	Input::getInstance()->removeListener(this);
	active = false;

	if(currentKey != -1 && capsMode == SINGLE){
		setCapsMode(LOWER);
	}
	btnRHeld = false;

	LoopManager::removeListener(this);
	currentKey = -1;
	keyTime = 0;
}

void TextEntry::focus(){
	lv_group_focus_obj(entry);
}

void TextEntry::defocus(){
	lv_obj_clear_state(entry, LV_STATE_FOCUSED);
}

void TextEntry::backspace(){
	lv_textarea_del_char(entry);
}

void TextEntry::keyPress(uint8_t i){
	if(i == BTN_L){
		backspace();
		return;
	}

	if(!keyMap.count(i)) return;
	uint8_t key = keyMap.at(i);
	const char* chars = characters[key];

	if(key == currentKey && keyTime != 0){
		index = (index + 1) % strnlen(chars, 16);
		char character = chars[index];
		if(capsMode == SINGLE || capsMode == UPPER){
			character = toUpperCase(character);
		}


		lv_textarea_del_char(entry);
		lv_textarea_add_char(entry, character);

		if(lv_obj_get_scroll_x(entry) > 0){
			lv_obj_scroll_to_x(entry, LV_COORD_MAX, LV_ANIM_OFF);
		}
	}else{
		if(getText().size() == lv_textarea_get_max_length(entry)) return;

		if(currentKey != -1 && currentKey != key && capsMode == SINGLE){
			setCapsMode(LOWER);
		}

		currentKey = key;
		index = 0;
		char character = chars[index];
		if(capsMode == SINGLE || capsMode == UPPER){
			character = toUpperCase(character);
		}

		lv_textarea_add_char(entry, character);
	}

	if(keyTime == 0){
		LoopManager::addListener(this);
	}

	keyTime = millis();
}

void TextEntry::buttonPressed(uint i){
	if(i == BTN_LEFT || i == BTN_RIGHT){
		if(currentKey != -1 && capsMode == SINGLE){
			setCapsMode(LOWER);
		}

		keyTime = 0;
		currentKey = -1;
		LoopManager::removeListener(this);
		lv_obj_set_style_anim_time(entry, 500, LV_PART_CURSOR | LV_STATE_FOCUSED);

		if(i == BTN_RIGHT){
			lv_textarea_cursor_right(entry);
		}else if(i == BTN_LEFT){
			lv_textarea_cursor_left(entry);
		}
		return;
	}

	if(i == BTN_ENTER || i == BTN_BACK) return;

	if(i == BTN_R) return;

	keyPress(i);
}

void TextEntry::buttonHeldRepeat(uint i, uint repeatCount){
	if(i == BTN_L){
		backspace();
	}else if(i == BTN_RIGHT){
		lv_textarea_cursor_right(entry);
	}else if(i == BTN_LEFT){
		lv_textarea_cursor_left(entry);
	}
}

void TextEntry::buttonHeld(uint i){
	if(i == BTN_R){
		btnRHeld = true;
		return;
	}

	if(!keyMap.count(i)) return;
	uint8_t key = keyMap.at(i);

	if(key != currentKey) return;

	const char* chars = characters[key];
	char last = chars[strnlen(chars, 16) - 1];

	lv_textarea_del_char(entry);
	lv_textarea_add_char(entry, last);

	keyTime = 0;
	currentKey = -1;
	LoopManager::removeListener(this);
}

void TextEntry::buttonReleased(uint i){
	if(i == BTN_LEFT || i == BTN_RIGHT){
		lv_async_call([](void* user_data){
			auto obj = static_cast<lv_obj_t*>(user_data);
			lv_event_send(obj, EV_ENTRY_LR, nullptr);
		}, obj);
	}

	if(i != BTN_R) return;

	if(btnRHeld){
		btnRHeld = false;
		return;
	}

	if(keyTime != 0){
		LoopManager::removeListener(this);
		keyTime = 0;
		currentKey = -1;
	}

	setCapsMode((CapsMode) ((capsMode + 1) % CapsMode::COUNT));
}

void TextEntry::loop(uint micros){
	if(millis() - keyTime < 600) return;

	keyTime = 0;
	currentKey = -1;
	lv_obj_set_style_anim_time(entry, 500, LV_PART_CURSOR | LV_STATE_FOCUSED);
	LoopManager::removeListener(this);

	if(capsMode == SINGLE){
		setCapsMode(LOWER);
	}
}

void TextEntry::setCapsMode(TextEntry::CapsMode mode){
	capsMode = mode;

	const char* CapsText[] = { "aa", "Aa", "AA" };
	lv_label_set_text(capsText, CapsText[mode]);
}

void TextEntry::showCaps(bool show){
	if(show){
		lv_obj_clear_flag(capsText, LV_OBJ_FLAG_HIDDEN);
	}else{
		lv_obj_add_flag(capsText, LV_OBJ_FLAG_HIDDEN);
	}
}
