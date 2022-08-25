
#include "CollisionComponent.h"
#include "CircleCC.h"
#include "RectCC.h"

CollisionComponent::CollisionComponent(CollisionType type) : type(type){

}

CollisionType CollisionComponent::getType() const{
	return type;
}

const CircleCC* CollisionComponent::getCircle() const{
	if(this->type != CollisionType::Circle) return nullptr;
	return reinterpret_cast<const CircleCC*>(this);
}

const RectCC* CollisionComponent::getRect() const{
	if(this->type != CollisionType::Rect) return nullptr;
	return reinterpret_cast<const RectCC*>(this);
}

const PolygonCC* CollisionComponent::getPolygon() const{
	if(this->type != CollisionType::Polygon) return nullptr;
	return reinterpret_cast<const PolygonCC*>(this);
}
