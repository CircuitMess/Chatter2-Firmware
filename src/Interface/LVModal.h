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

	static LVModal* getCurrent();
protected:
	LVScreen* parentScreen;
	lv_group_t* inputGroup;

	lv_obj_t* container;

	bool active = false;

	virtual void onStart();
	virtual void onStop();

private:
	static LVModal* current;

};


#endif //CHATTER_FIRMWARE_LVMODAL_H
