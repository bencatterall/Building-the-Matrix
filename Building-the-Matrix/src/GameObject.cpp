#include "GameObject.hpp"
#include "ObjectManager.hpp"
#include "LocationComponent.hpp"
GameObject::GameObject(bool renderable) : renderable(renderable) {
	//Get a GUID from the ObjectManager
	this->ID = ObjectManager::getNextID();

	if (renderable) {
		this->renderableComponent = std::make_shared<RenderableComponent>();
		this->locationComponent = std::make_shared<LocationComponent>();
	}
}

GameObject::GameObject() : GameObject(false) {

}

GameObject::~GameObject() {

}