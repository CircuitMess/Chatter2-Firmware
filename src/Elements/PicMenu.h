#ifndef CHATTER_FIRMWARE_PICMENU_H
#define CHATTER_FIRMWARE_PICMENU_H


#include <vector>
#include "../Interface/LVObject.h"
#include "../Interface/LVModal.h"

class PicMenu : public LVModal {
public:
	PicMenu(LVScreen* parent);

	uint8_t getSelected();

protected:
	void onStart() override;
	void onStop() override;

private:
	lv_obj_t* picLayout;
	std::vector<lv_obj_t*> pics;

	lv_obj_t* arrowLeft;
	lv_obj_t* arrowRight;
	lv_anim_t anim;

	uint8_t index = 0;

	void refreshArrows();

};


#endif //CHATTER_FIRMWARE_PICMENU_H
