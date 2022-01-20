#ifndef CHATTER_FIRMWARE_CONTEXTMENU_H
#define CHATTER_FIRMWARE_CONTEXTMENU_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVObject.h"
#include <string>
#include <vector>
#include <Input/InputListener.h>

class ContextMenu : public LVObject {
public:
	ContextMenu(lv_obj_t* parent, const std::vector<std::string>& options);

private:
	std::vector<lv_obj_t*> labelVector;
	lv_obj_t* labelObj;
	uint8_t num = 0;
	void down();
	void up();
protected:
	lv_style_t styleFocus;
	lv_style_t styleDef;
};

#endif //CHATTER_FIRMWARE_CONTEXTMENU_H

