#include "GameObject.hpp"
#include "ObjectManager.hpp"
GameObject::GameObject() {
	//Get a GUID for the ObjectManager
	this->ID = ObjectManager::getNextID();
}

GameObject::~GameObject() {

}