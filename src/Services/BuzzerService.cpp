#include "BuzzerService.h"
#include <Pins.hpp>
#include <Audio/Piezo.h>
#include <Notes.h>
#include <Loop/LoopManager.h>
#include <Input/Input.h>
#include <Settings.h>

BuzzerService Buzz;


void BuzzerService::begin(){
	Messages.addReceivedListener(this);
	Input::getInstance()->addListener(this);
}

const std::unordered_map<uint8_t, uint16_t> BuzzerService::noteMap = {
		{BTN_1, NOTE_C4},
		{BTN_2, NOTE_D4},
		{BTN_3, NOTE_E4},
		{BTN_4, NOTE_F4},
		{BTN_5, NOTE_G4},
		{BTN_6, NOTE_A4},
		{BTN_7, NOTE_B4},
		{BTN_8, NOTE_C5},
		{BTN_9, NOTE_D5},
		{BTN_L, NOTE_E5},
		{BTN_0, NOTE_F5},
		{BTN_R, NOTE_G5},

		{BTN_LEFT, NOTE_B4},
		{BTN_RIGHT, NOTE_B4},
		{BTN_ENTER, NOTE_C5},
		{BTN_B, NOTE_A4}
};

const std::vector<BuzzerService::Note> BuzzerService::Notes = {
		{ NOTE_B5, 1000000 },
		{ 0, 100000 },
		{ NOTE_B4, 100000 }
};


void BuzzerService::msgReceived(const Message &message){
	if(!Settings.get().sound) return;
	if(message.convo == noBuzzUID && noBuzzUID != ESP.getEfuseMac()) return;

	LoopManager::addListener(this);

	noteIndex = 0;
	noteTime = 0;
	Piezo.tone(Notes[noteIndex].freq);
}

void BuzzerService::setNoBuzzUID(UID_t noBuzzUid){
	noBuzzUID = noBuzzUid;
}

void BuzzerService::buttonPressed(uint i){
	if(i == BTN_ENTER && muteEnter) return;
	if(!Settings.get().sound) return;
	Piezo.tone(noteMap.at(i), 25);
}

void BuzzerService::loop(uint micros){
	if(!Settings.get().sound){
		Piezo.noTone();
		LoopManager::removeListener(this);
		return;
	}

	noteTime += micros;
	if(noteTime < Notes[noteIndex].duration) return;

	noteIndex++;
	noteTime = 0;

	if(noteIndex >= Notes.size()){
		LoopManager::removeListener(this);
		Piezo.noTone();
		return;
	}

	if(Notes[noteIndex].freq == 0){
		Piezo.noTone();
		return;
	}


	Piezo.tone(Notes[noteIndex].freq);
}

void BuzzerService::setMuteEnter(bool muteEnter){
	BuzzerService::muteEnter = muteEnter;
}


