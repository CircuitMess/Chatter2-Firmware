#ifndef CHATTER_FIRMWARE_BUZZERSERVICE_H
#define CHATTER_FIRMWARE_BUZZERSERVICE_H

#include <Input/InputListener.h>
#include "MessageService.h"

class BuzzerService : public InputListener, private MsgReceivedListener, public LoopListener  {
public:
	void begin();
	void loop(uint micros) override;
	void setNoBuzzUID(UID_t noBuzzUid);

	void setMuteEnter(bool muteEnter);
private:
	void buttonPressed(uint i) override;
	void msgReceived(const Message &message) override;

	UID_t noBuzzUID = ESP.getEfuseMac();

	static const std::unordered_map<uint8_t, uint16_t> noteMap;
	struct Note { uint16_t freq; uint32_t duration; };
	static const std::vector<Note> Notes;

	uint32_t noteTime = 0;
	uint8_t noteIndex = 0;
	bool muteEnter = false;
};

extern BuzzerService Buzz;
#endif //CHATTER_FIRMWARE_BUZZERSERVICE_H
