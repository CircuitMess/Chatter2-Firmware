#include "GameObject.h"

GameObject::GameObject(std::unique_ptr<RenderComponent> rc, std::unique_ptr<CollisionComponent> cc) : renderComponent(std::move(rc)),
																									  collisionComponent(std::move(cc)){
}

glm::vec2 GameObject::getPos() const{
	return pos;
}

void GameObject::setPos(glm::vec2 pos){
	GameObject::pos = pos;
}

std::shared_ptr<RenderComponent> GameObject::getRenderComponent() const{
	return renderComponent;
}

std::shared_ptr<CollisionComponent> GameObject::getCollisionComponent() const{
	return collisionComponent;
}

float GameObject::getRot() const{
	return rot;
}

void GameObject::setRot(float rot){
	GameObject::rot = rot;
}


