#include "Player.h"

void Player::leftTurn(float delta){
	angle -= delta * rotSpeed;
	angle = fmod(angle, 360.0f);

	obj->setRot(angle);
}

void Player::rightTurn(float delta){
	angle += delta * rotSpeed;
	angle = fmod(angle, 360.0f);

	obj->setRot(angle);
}

float Player::getAngle() const{
	return angle;
}

void Player::setObj(Player::ObjPtr obj){
	Player::obj = obj;

}

const Player::ObjPtr& Player::getObj() const{
	return obj;
}

