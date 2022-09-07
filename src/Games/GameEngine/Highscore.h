#ifndef CHATTER_HIGHSCORE_H
#define CHATTER_HIGHSCORE_H

#include <Arduino.h>
#include <nvs.h>

#define HS_NAME_LIMIT 3 // player name character limit
#define HS_MAX_SCORES 5 // # of players in HS

class Highscore {
public:
	virtual ~Highscore();

	struct Score {
		char name[HS_NAME_LIMIT + 1];
		uint score;
	};

	void begin(const char* name);
	void add(const Score& score);
	const Score& get(uint8_t i);
	void clear();
	uint8_t count();

private:
	struct Data {
		uint8_t count;
		Score scores[HS_MAX_SCORES];
	} data;

	void save();
	void load();

	nvs_handle nvs = 0;
};


#endif // CHATTER_HIGHSCORE_H