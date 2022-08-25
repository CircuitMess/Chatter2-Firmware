
#ifndef CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H
#define CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H

#include <memory>
#include <iostream>

class RectCC;

class CircleCC;

class PolygonCC;

enum class CollisionType {
	Circle, Rect, Polygon
};

class CollisionComponent {
protected:
	CollisionType type;
	CollisionComponent(CollisionType type);

public:
	virtual ~CollisionComponent() = default;
	CollisionType getType() const;
	const CircleCC* getCircle() const;
	const RectCC* getRect() const;
	const PolygonCC* getPolygon() const;
};


#endif //CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H
