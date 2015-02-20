#include "GameObject.hpp"

GameObject::GameObject(GameObjectGlobalID id) {
	(this->ID) = id;
	(this->userControllable) = false;
	(this->deleted) = false;
}

GameObject::GameObject(const GameObject& other) {
	//copy
	(this->ID) = other.ID;
	(this->xrot) = other.xrot;
	(this->yrot) = other.yrot;
	(this->zrot) = other.zrot;
	(this->xpos) = other.xpos;
	(this->ypos) = other.ypos;
	(this->zpos) = other.zpos;
	(this->visible) = other.visible;
	(this->renderable) = other.renderable;
	(this->deleted) = other.deleted;
	(this->userControllable) = other.userControllable;
}

GameObject::GameObject() {}

GameObject::~GameObject() {}

GameObjectGlobalID GameObject::getID(){
	return this->ID;
}