#include <vector>
#include <Pins.hpp>
#include "UserHWTest.h"
#include <Input/Input.h>
#include <Notes.h>
#include <Loop/LoopManager.h>
#include <Audio/Piezo.h>
#include "../Fonts/font.h"

const std::unordered_map<uint8_t, UserHWTest::KeyCoord> UserHWTest::KeyCoords = {
		{ BTN_LEFT, { 13, 74 } }, { BTN_RIGHT, { 28, 74 } }, { BTN_ENTER, { 43, 74 } }, { BTN_BACK, { 58, 74 } },
		{ BTN_1, { 13, 88 } }, { BTN_2, { 28, 88 } }, { BTN_3, { 43, 88 } },
		{ BTN_4, { 13, 98 } }, { BTN_5, { 28, 98 } }, { BTN_6, { 43, 98 } },
		{ BTN_7, { 13, 108 } }, { BTN_8, { 28, 108 } }, { BTN_9, { 43, 108 } },
		{ BTN_L, { 13, 118 } }, { BTN_0, { 28, 118 } }, { BTN_R, { 43, 118 } }
};

lv_color_t* UserHWTest::canvasBuffer = nullptr;
bool UserHWTest::usingCanvas = false;

const std::vector<UserHWTest::Note> UserHWTest::Notes = {
		{ NOTE_C4, 400 },
		{ NOTE_E4, 400 },
		{ NOTE_G4, 400 },
		{ NOTE_C5, 600 },
		{ NOTE_C5, 400 },
		{ NOTE_G4, 400 },
		{ NOTE_E4, 400 },
		{ NOTE_C4, 600 }
};

UserHWTest::UserHWTest(void (*doneCallback)()) : doneCallback(doneCallback){
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, 0);
	lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_t* imgContainer = lv_obj_create(obj);
	lv_obj_clear_flag(imgContainer, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_size(imgContainer, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_style_pad_right(imgContainer, 3, 0);
	lv_obj_set_style_pad_left(imgContainer, 1, 0);

	lv_obj_t* img = lv_img_create(imgContainer);
	lv_obj_clear_flag(img, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_scrollbar_mode(img, LV_SCROLLBAR_MODE_OFF);
	lv_img_set_src(img, "S:/UserHW.bin");

	canvas = lv_canvas_create(imgContainer);
	lv_obj_set_size(canvas, 75, 128);

	if(canvasBuffer){
		if(usingCanvas){
			printf("User HW Test: Canvas buffer already in use!\n");
		}else{
			usingCanvas = true;
			lv_canvas_set_buffer(canvas, canvasBuffer, 75, 128, LV_IMG_CF_TRUE_COLOR_ALPHA);
			lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_0);
		}
	}else{
		canvasBuffer = static_cast<lv_color_t*>(malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(75, 128)));
		lv_canvas_set_buffer(canvas, canvasBuffer, 75, 128, LV_IMG_CF_TRUE_COLOR_ALPHA);
		lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_0);
		usingCanvas = true;
	}

	lv_obj_t* container = lv_obj_create(obj);
	lv_obj_set_layout(container, LV_LAYOUT_FLEX);
	lv_obj_set_style_flex_flow(container, LV_FLEX_FLOW_COLUMN, 0);
	lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_height(container, lv_pct(100));
	lv_obj_set_flex_grow(container, 1);
	lv_obj_set_style_pad_top(container, 10, 0);

	lv_obj_t* title = lv_label_create(container);
	lv_obj_set_width(title, lv_pct(100));
	lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_style_text_font(title, &pixelbasic7, 0);
	lv_obj_set_style_text_color(title, lv_color_hex(0xb6d53c), 0);
	lv_obj_set_style_pad_bottom(title, 15, 0);
	lv_label_set_text(title, "Hardware Test");

	for(const char* text : std::vector<const char*>{
		"Press buttons to test them.",
		"You should hear a melody playing on the buzzer.\n\nPress any key to exit test."
	}){
		labels.push_back(lv_label_create(container));
		lv_obj_t* label = labels.back();

		lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
		lv_label_set_text(label, text);
		lv_obj_set_size(label, lv_pct(100), LV_SIZE_CONTENT);
		lv_obj_set_style_text_font(label, &pixelbasic7, 0);
		lv_obj_set_style_text_line_space(label, 5, 0);
		lv_obj_set_style_text_color(label, lv_color_white(), 0);
		lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
	}
	lv_obj_clear_flag(labels.front(), LV_OBJ_FLAG_HIDDEN);

	for(int i = 0; i < buttons.size(); i++){
		buttons[i] = false;
	}
}

UserHWTest::~UserHWTest(){
	if(!usingCanvas){
		free(canvasBuffer);
		canvasBuffer = nullptr;
	}
}

void UserHWTest::onStart(){
	Input::getInstance()->addListener(this);
}

void UserHWTest::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
	Piezo.noTone();
	usingCanvas = false;
}

void UserHWTest::buttonPressed(uint i){
	if(stage == 1){
		onStop();

		if(doneCallback){
			stop();
			volatile auto callback = doneCallback;
			delete this;
			callback();
		}else{
			pop();
		}
		return;
	}

	if(stage != 0) return;

	if(i >= buttons.size()) return;
	if(buttons[i]) return;

	auto pair = KeyCoords.find(i);
	if(pair == KeyCoords.end()) return;

	KeyCoord coord = pair->second;

	buttons[i] = true;
	buttonCount++;

	lv_draw_rect_dsc_t lineInfo;
	lv_draw_rect_dsc_init(&lineInfo);
	lineInfo.bg_color = lv_color_hex(0x00ee00);
	lineInfo.bg_opa = LV_OPA_COVER;
	lineInfo.radius = LV_RADIUS_CIRCLE;
	lv_canvas_draw_rect(canvas, coord.x - 3, coord.y - 3, 7, 7, &lineInfo);

	if(buttonCount == 16){
		startBuzz();
	}
}

void UserHWTest::startBuzz(){
	if(stage == 1) return;

	stage = 1;
	lv_obj_add_flag(labels[0], LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(labels[1], LV_OBJ_FLAG_HIDDEN);

	LoopManager::addListener(this);
	noteIndex = 0;
	noteTime = millis();
	Piezo.tone(Notes[noteIndex].freq);
}

void UserHWTest::loop(uint micros){
	if(millis() - noteTime < Notes[noteIndex].duration) return;

	noteIndex = (noteIndex + 1) % Notes.size();
	noteTime = millis();
	Piezo.tone(Notes[noteIndex].freq);
}
