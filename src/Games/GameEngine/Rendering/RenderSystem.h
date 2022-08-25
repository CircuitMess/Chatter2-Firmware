#ifndef CIRCUITPET_FIRMWARE_RENDERSYSTEM_H
#define CIRCUITPET_FIRMWARE_RENDERSYSTEM_H

#include <Display/Sprite.h>
#include "../GameSystem.h"

class RenderSystem : public GameSystem {
public:
	RenderSystem(const Game* game, Sprite* canvas);

	void update(uint32_t deltaMicros) override;

private:
	Sprite* canvas;
};


#endif //CIRCUITPET_FIRMWARE_RENDERSYSTEM_H
