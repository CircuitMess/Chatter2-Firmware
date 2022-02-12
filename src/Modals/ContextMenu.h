#ifndef CHATTER_FIRMWARE_CONTEXTMENU_H
#define CHATTER_FIRMWARE_CONTEXTMENU_H

#include <Arduino.h>
#include <lvgl.h>
#include "../Interface/LVObject.h"
#include "../Interface/LVModal.h"
#include <string>
#include <vector>
#include <Input/InputListener.h>

class ContextMenu : public LVModal {
public:
	struct Option {
		std::string text;
		int16_t value;
	};

	ContextMenu(LVScreen* parent, const std::vector<Option>& options = {  });
	virtual ~ContextMenu();
	void setOptions(const std::vector<Option>& options);
	const Option& getSelected();

protected:
	void onStart() override;

private:
	std::vector<Option> options;
	std::vector<lv_obj_t*> labels;
	lv_obj_t* label;

	lv_style_t styleFocus;
	lv_style_t styleDef;

};

#endif //CHATTER_FIRMWARE_CONTEXTMENU_H

