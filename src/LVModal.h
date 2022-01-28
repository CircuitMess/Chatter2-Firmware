#ifndef CHATTER_FIRMWARE_LVMODAL_H
#define CHATTER_FIRMWARE_LVMODAL_H


#include "LVObject.h"
#include "LVScreen.h"

class LVModal : public LVObject {
public:
	LVModal(LVScreen* parent);
	virtual ~LVModal();

	void start();
	void stop();

	bool isActive() const;

protected:
	LVScreen* parentScreen;
	lv_group_t* inputGroup;

	bool active = false;

};


#endif //CHATTER_FIRMWARE_LVMODAL_H
