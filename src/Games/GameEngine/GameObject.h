#ifndef CIRCUITPET_FIRMWARE_GAMEOBJECT_H
#define CIRCUITPET_FIRMWARE_GAMEOBJECT_H

#include <memory>
#include "Rendering/RenderComponent.h"
#include "PixelDim.hpp"
#include "Collision/CollisionComponent.h"

class GameObject {
public:
	GameObject(std::unique_ptr<RenderComponent> rc, std::unique_ptr<CollisionComponent> cc);

	virtual ~GameObject() = default;
	glm::vec2 getPos() const;
	void setPos(glm::vec2 pos);

	float getRot() const;
	void setRot(float rot);

	//TODO - razraditi ovo i implementirati
	void clone();

	std::shared_ptr<RenderComponent> getRenderComponent() const;
	std::shared_ptr<CollisionComponent> getCollisionComponent() const;

private:
	glm::vec2 pos = { 0, 0 };
	float rot = 0;
	std::shared_ptr<RenderComponent> renderComponent;
	std::shared_ptr<CollisionComponent> collisionComponent;

};


#endif //CIRCUITPET_FIRMWARE_GAMEOBJECT_H
