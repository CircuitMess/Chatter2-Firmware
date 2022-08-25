#include "RenderComponent.h"

int8_t RenderComponent::getLayer() const{
	return layer;
}

void RenderComponent::setLayer(int8_t layer){
	RenderComponent::layer = layer;
}

bool RenderComponent::isVisible() const{
	return visible;
}

void RenderComponent::setVisible(bool visible){
	RenderComponent::visible = visible;
}
