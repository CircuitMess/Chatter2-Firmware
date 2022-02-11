#ifndef CHATTER_FIRMWARE_BATTERYNOTIFICATION_H
#define CHATTER_FIRMWARE_BATTERYNOTIFICATION_H

#include "../../LVModal.h"


class BatteryNotification : public LVModal {
public:
	enum Type {
		WARNING, SHUTDOWN
	};
	BatteryNotification(LVScreen* parent, Type type);
protected:
	void onStart() override;
	void onStop() override;
};


#endif //CHATTER_FIRMWARE_BATTERYNOTIFICATION_H
