#include "Highscore.h"

#include <FS.h>

void Highscore::begin(const char* name){
	bool firstTime = false;

	auto err = nvs_open(name, NVS_READONLY, &nvs);
	if(err == ESP_ERR_NVS_NOT_FOUND){
		firstTime = true;
	}else if(err == ESP_OK){
		nvs_close(nvs);
	}else{
		ESP_LOGE("HighScore", "Can't open readonly NVS entry for %s", name);
		return;
	}

	err = nvs_open(name, NVS_READWRITE, &nvs);
	if(err != ESP_OK){
		ESP_LOGE("HighScore", "Can't open readwrite NVS entry for %s", name);
		return;
	}

	if(firstTime){
		data.count = 0;
		save();
	}else{
		load();
	}
}

Highscore::~Highscore(){
	nvs_close(nvs);
}

void Highscore::add(const Score& score){
	if(data.count == HS_MAX_SCORES && score.score < data.scores[data.count-1].score) return; // HS full, new score is lower than last in HS

	uint8_t i;
	for(i = 0; i < data.count; i++){
		if(score.score <= data.scores[i].score) continue; // if score is lower or equal, continue

		// otherwise, move others down
		uint8_t first = i;
		uint8_t count = data.count - i;
		if(data.count == HS_MAX_SCORES) count--; // if HS is full, we're moving one score less, so that we don't write over space that isn't part of the HS

		// move
		// memcpy(&data.scores[first+1], &data.scores[first], count * sizeof(Score));
		for(int j = first + count; j > first; j--){
			memcpy(&data.scores[j], &data.scores[j-1], sizeof(Score));
		}

		break;
	}

	if(i == HS_MAX_SCORES) return; // all scores are higher than the new one

	// write the new score
	memcpy(&data.scores[i], &score, sizeof(Score));
	data.count = min(HS_MAX_SCORES, data.count+1);
	save();
}

void Highscore::clear(){
	data.count = 0;
	save();
}

const Highscore::Score& Highscore::get(uint8_t i){
	return data.scores[i];
}

uint8_t Highscore::count(){
	return data.count;
}

void Highscore::save(){
	if(nvs == 0) return;

	auto err = nvs_set_blob(nvs, "HS", &data, sizeof(Data));
	if(err != ESP_OK){
		ESP_LOGE("HighScore", "Load fail");
	}
}

void Highscore::load(){
	if(nvs == 0) return;

	size_t size;
	auto err = nvs_get_blob(nvs, "HS", &data, &size);
	if(err == ESP_ERR_NVS_NOT_FOUND){
		clear();
	}else if(err != ESP_OK){
		ESP_LOGE("HighScore", "Load fail");
	}else if(size == 0){
		clear();
	}
}
