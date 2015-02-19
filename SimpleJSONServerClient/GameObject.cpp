#include "GameObject.hpp"

GameObject::GameObject(GameObjectGlobalID id, bool userObj) {
	(this->ID) = id;
	(this->userControllable) = userObj;
}

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::keyPressed(char key) {
	if (this->userControllable) {
		(this->control).regKeyPress(key);
	}
}

void GameObject::setPRY(float pitch, float roll, float yaw) {
	if (this->userControllable) {
		(this->pitch) = pitch;
		(this->roll) = roll;
		(this->yaw) = yaw;
	}
}

void GameObject::keyUnpressed(char key) {
	if (this->userControllable) {
		(this->control).regKeyUnpress(key);
	}
}

GameObjectGlobalID GameObject::getID(){
	return this->ID;
}