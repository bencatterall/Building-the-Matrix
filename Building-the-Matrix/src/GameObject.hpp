#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Common.hpp"
#include <memory>

class LocationComponent;
class RenderableComponent;
class PhysicsObject;

class GameObject {

	///
	/// Global ID of this object as held by the server
	///
	GameObjectGlobalID globalID = 0;

	///
	///Unique ID used to refer to the object
	///
	GameObjectID ID = 0;

	bool renderable;
	std::shared_ptr<RenderableComponent> renderableComponent;
	std::shared_ptr<LocationComponent> locationComponent;
	std::shared_ptr<PhysicsObject> physicsObject;
public:
	///Objects are renderable by default
	GameObject();
	GameObject(bool renderable);
	~GameObject();

	std::shared_ptr<LocationComponent> getLocationComponent() { return locationComponent;  }
	std::shared_ptr<RenderableComponent> getRenderableComponent() { return renderableComponent; }
	std::shared_ptr<PhysicsObject> getPhysicsComponent() { return physicsObject; }

	bool isRenderable() { return renderable; }
	int getID() { return ID;  }

	int setGlobalID(int globalID) {
		this->globalID = globalID;
	}
	int getGlobalID() {
		return globalID;
	}
};


#endif