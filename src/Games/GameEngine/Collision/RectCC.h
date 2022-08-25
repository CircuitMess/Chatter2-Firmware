
#ifndef CIRCUITPET_FIRMWARE_RECTCC_H
#define CIRCUITPET_FIRMWARE_RECTCC_H
#include "CollisionComponent.h"
#include "../PixelDim.hpp"

class RectCC : public CollisionComponent{
public:
	RectCC(glm::vec2 dim);
	~RectCC() override = default;

	glm::vec2 getDim() const;

private:
	glm::vec2 dim;
};


#endif //CIRCUITPET_FIRMWARE_RECTCC_H
