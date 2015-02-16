#include "GameObject.hpp"
#include "ObjectManager.hpp"
#include "LocationComponent.hpp"
#include "RenderableComponent.hpp"
#include "JSON/UpdateManager.hpp"
#include "Physics\PhysicsObject.hpp"

GameObject::GameObject(bool renderable, bool visible) :
	renderable(renderable), visible(visible) {
	//Get a GUID from the ObjectManager
	this->ID = ObjectManager::getNextID();

	if (renderable) {
		this->renderableComponent = std::make_shared<RenderableComponent>();
		this->locationComponent = std::make_shared<LocationComponent>();
		this->physicsObject = std::make_shared<PhysicsObject>(this->locationComponent, this->renderableComponent);
	}
}

GameObject::GameObject() : 
	GameObject(true, true) {

}

GameObject::~GameObject() {

}