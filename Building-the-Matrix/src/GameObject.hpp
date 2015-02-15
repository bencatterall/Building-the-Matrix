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

	bool visible;
	bool renderable;
	std::shared_ptr<RenderableComponent> renderableComponent;
	std::shared_ptr<LocationComponent> locationComponent;
	std::shared_ptr<PhysicsObject> physicsObject;
public:
	///Objects are renderable by default
	GameObject();
	GameObject(bool renderable, bool visible);
	~GameObject();

	std::shared_ptr<LocationComponent> getLocationComponent() { return locationComponent;  }
	std::shared_ptr<RenderableComponent> getRenderableComponent() { return renderableComponent; }
	void setRenderableComponent(std::shared_ptr<RenderableComponent> renderComp) { renderableComponent = renderComp; }
	std::shared_ptr<PhysicsObject> getPhysicsComponent() { return physicsObject; }

	bool isRenderable() { return renderable; }
	bool isVisible() { return visible; }
	void setVisible(bool visible) { this->visible = visible; }
	GameObjectID getID() { return ID; }

	void setGlobalID(GameObjectID globalID) {
		this->globalID = globalID;
	}
	GameObjectID getGlobalID() {
		return globalID;
	}
};


#endif