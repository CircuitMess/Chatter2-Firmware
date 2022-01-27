#ifndef CHATTER_FIRMWARE_CONTEXTMENU_H
#define CHATTER_FIRMWARE_CONTEXTMENU_H

#include <Arduino.h>
#include <lvgl.h>
#include "../LVObject.h"
#include "../LVModal.h"
#include <string>
#include <vector>
#include <Input/InputListener.h>

class ContextMenu : public LVModal {
public:
	ContextMenu(LVScreen* parent, const std::vector<std::string>& options);

private:
	std::vector<lv_obj_t*> labelVector;
	lv_obj_t* labelObj;

	lv_style_t styleFocus;
	lv_style_t styleDef;

};

#endif //CHATTER_FIRMWARE_CONTEXTMENU_H

