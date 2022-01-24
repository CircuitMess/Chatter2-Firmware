#include "Battery.h"

Battery::Battery(lv_obj_t* parent) : LVObject(parent){
	lv_obj_set_size(obj, 19, 12);
	img = lv_img_create(obj);

	lv_img_set_src(img, (String("S:/Battery/") + level + ".bin").c_str());
}
