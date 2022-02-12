#ifndef CHATTER_FIRMWARE_PROMPT_H
#define CHATTER_FIRMWARE_PROMPT_H

#include "../Interface/LVModal.h"

#define EV_PROMPT_YES ((lv_event_code_t) (_LV_EVENT_LAST + 1))
#define EV_PROMPT_NO ((lv_event_code_t) (_LV_EVENT_LAST + 2))

class Prompt : public LVModal{
public:
	Prompt(LVScreen* parent, const char* text);
	virtual ~Prompt();

private:
	lv_style_t btnDefault;
	lv_style_t btnFocused;

	lv_obj_t* noBtn;
	lv_obj_t* yesBtn;
};


#endif //CHATTER_FIRMWARE_PROMPT_H
