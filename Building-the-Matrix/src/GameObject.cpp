#include "GameObject.hpp"
#include "ObjectManager.hpp"
#include "LocationComponent.hpp"
#include "RenderableComponent.hpp"
#include "JSON/UpdateManager.hpp"
#include "Physics/PhysicsObject.hpp"
#include "Serializer.hpp"

GameObject::GameObject(glm::vec3 worldlLoc) :
	GameObject(true, true, worldlLoc) {
}

GameObject::GameObject(bool renderable, bool visible, glm::vec3 worldLoc) :
	renderable(renderable), visible(visible) {
	//Get a GUID from the ObjectManager
	this->ID = ObjectManager::getNextID();

	if (renderable) {
		this->renderableComponent = std::make_shared<RenderableComponent>();
		this->locationComponent = std::make_shared<LocationComponent>(worldLoc);
	}
}

GameObject::GameObject() : 
	GameObject(true, true) {

}

int GameObject::deserialize(unsigned char* buffer){
	Serializer serializer;
	int next = 0;
	(this->ID) = serializer.unpackInt(buffer, next);
	(this->physicsObject) = std::make_shared<PhysicsObject>(&buffer[next], next);
	(this->visible) = serializer.unpackBool(&buffer[next], next);
	(this->renderable) = serializer.unpackBool(&buffer[next], next);
	(this->locationComponent) = (this->physicsObject)->getLocationComponent();
	return next;
}

GameObject::~GameObject() {

}