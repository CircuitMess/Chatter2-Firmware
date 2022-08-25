
#ifndef CIRCUITPET_FIRMWARE_CIRCLECC_H
#define CIRCUITPET_FIRMWARE_CIRCLECC_H

#include <cstdint>
#include "CollisionComponent.h"
#include "../PixelDim.hpp"

class CircleCC : public CollisionComponent{
public:
	CircleCC(float radius, glm::vec2 offset = {0, 0});
	~CircleCC() override = default;

	float getRadius() const;
	glm::vec2 getOffset() const;
	void setOffset(const glm::vec2& offset);

private:
	float radius;
	glm::vec2 offset;
};


#endif //CIRCUITPET_FIRMWARE_CIRCLECC_H
