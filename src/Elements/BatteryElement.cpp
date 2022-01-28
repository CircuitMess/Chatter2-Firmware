#include "BatteryElement.h"

BatteryElement::BatteryElement(lv_obj_t* parent) : LVObject(parent){
	lv_obj_set_size(obj, 19, 12);
	img = lv_img_create(obj);

	level = Battery.getLevel();
	lv_img_set_src(img, (String("S:/Battery/") + level + ".bin").c_str());

	LoopManager::addListener(this);
}

void BatteryElement::loop(uint micros){
	uint8_t newLevel = Battery.getLevel();
	if(newLevel != level){
		level = newLevel;
		lv_img_set_src(img, (String("S:/Battery/") + level + ".bin").c_str());
	}

}

BatteryElement::~BatteryElement(){
	LoopManager::removeListener(this);
}
