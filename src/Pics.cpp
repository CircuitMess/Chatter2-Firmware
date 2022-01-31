#include "Pics.h"

const Pic Pics[NUM_PICS] = {
		{ "0.bin" },
		// { "1.gif" },
		{ "2.bin" },
		// { "3.gif" },
		{ "4.bin" },
		{ "5.bin" },
		// { "6.gif" },
		// { "7.gif" },
		{ "8.bin" },
		// { "9.gif" },
		{ "10.bin" },
		{ "11.bin" },
		{ "12.bin" },
		// { "13.gif" },
		{ "14.bin" }
};

bool Pic::gif() const{
	String str = String(name);
	str.toLowerCase();
	return str.endsWith(".gif");
}

lv_obj_t* Pic::create(lv_obj_t* parent) const{
	String path = String("S:/Pics/") + name;

	lv_obj_t* pic;
	if(gif()){
		pic = lv_gif_create(parent);
		lv_gif_set_src(pic, path.c_str());
	}else{
		pic = lv_img_create(parent);
		lv_img_set_src(pic, path.c_str());
	}

	lv_obj_set_size(pic, 64, 64);

	return pic;
}
