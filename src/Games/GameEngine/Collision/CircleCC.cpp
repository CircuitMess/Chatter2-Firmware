
#include "CircleCC.h"

CircleCC::CircleCC(float radius, glm::vec2 offset): radius(radius), offset(offset), CollisionComponent(CollisionType::Circle){

}

float CircleCC::getRadius() const{
	return radius;
}

glm::vec2 CircleCC::getOffset() const{
	return offset;
}

void CircleCC::setOffset(const glm::vec2& offset){
	CircleCC::offset = offset;
}
