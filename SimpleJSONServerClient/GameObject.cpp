#include "GameObject.hpp"

GameObject::GameObject(GameObjectGlobalID id,bool userObj) {
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

void GameObject::keyUnpressed(char key) {
	if (this->userControllable) {
		(this->control).regKeyUnpress(key);
	}
}

GameObjectGlobalID GameObject::getID(){
	return this->ID;
}