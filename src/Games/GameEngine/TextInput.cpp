#include "TextInput.h"
#include <utility>
#include <Input/Input.h>
#include <Pins.hpp>
#include <Loop/LoopManager.h>

const char* TextInput::characters[] = {
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

const std::map<uint8_t, uint8_t> TextInput::keyMap = {
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

void TextInput::start(){
	Input::getInstance()->addListener(this);
}

void TextInput::stop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void TextInput::setCallbacks(std::function<void()> onNextChar, std::function<void()> onPrevChar, std::function<void(char)> onSetChar){
	this->onNextChar = std::move(onNextChar);
	this->onPrevChar = std::move(onPrevChar);
	this->onSetChar = std::move(onSetChar);
}

void TextInput::keyPress(uint8_t i){
	if(i == BTN_L){
		keyTime = 0;
		currentKey = -1;
		LoopManager::removeListener(this);

		onSetChar(' ');
		onPrevChar();
		return;
	}

	if(!keyMap.count(i)) return;
	uint8_t key = keyMap.at(i);
	const char* chars = characters[key];

	if(key == currentKey && keyTime != 0){
		index = (index + 1) % strnlen(chars, 16);

		char character = chars[index];
		onSetChar(character);
	}else{
		if(currentKey != -1){
			onNextChar();
		}

		currentKey = key;
		index = 0;

		char character = chars[index];
		onSetChar(character);
	}

	if(keyTime == 0){
		LoopManager::addListener(this);
	}

	keyTime = millis();
}

void TextInput::buttonPressed(uint i){
	if(i == BTN_LEFT || i == BTN_RIGHT){
		keyTime = 0;
		currentKey = -1;
		LoopManager::removeListener(this);

		if(i == BTN_RIGHT){
			onNextChar();
		}else if(i == BTN_LEFT){
			onPrevChar();
		}

		return;
	}

	if(i == BTN_ENTER || i == BTN_BACK) return;

	if(i == BTN_R) return;

	keyPress(i);
}

void TextInput::loop(uint micros){
	if(millis() - keyTime < 1000) return;

	keyTime = 0;
	currentKey = -1;
	LoopManager::removeListener(this);

	onNextChar();
}
