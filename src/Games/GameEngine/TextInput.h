#ifndef CHATTER2_FIRMWARE_TEXTINPUT_H
#define CHATTER2_FIRMWARE_TEXTINPUT_H

#include <Arduino.h>
#include <functional>
#include <Input/InputListener.h>
#include <Loop/LoopListener.h>

class TextInput : private InputListener, private LoopListener {
public:
	void setCallbacks(
			std::function<void()> onNextChar,
			std::function<void()> onPrevChar,
			std::function<void(char)> onSetChar
	);

	void start();
	void stop();

private:
	void buttonPressed(uint i) override;
	void loop(uint micros) override;

	std::function<void()> onNextChar;
	std::function<void()> onPrevChar;
	std::function<void(char)> onSetChar;

	static const char* characters[];
	static const std::map<uint8_t, uint8_t> keyMap;

	void keyPress(uint8_t i);
	int8_t currentKey = -1; // currently active key
	uint8_t index = 0; // character under the key
	uint32_t keyTime = 0; // when the key was last pressed

};


#endif //CHATTER2_FIRMWARE_TEXTINPUT_H
