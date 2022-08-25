
#include "RectCC.h"

RectCC::RectCC(glm::vec2 dim) : dim(dim), CollisionComponent(CollisionType::Rect){

}

glm::vec2 RectCC::getDim() const{
	return dim;
}
