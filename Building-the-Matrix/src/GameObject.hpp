#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Common.hpp"
#include "LocationComponent.hpp"
#include "RenderableComponent.hpp"
#include "JSON/UpdateManager.hpp"
#include <memory>

class GameObject {

	///
	/// Global ID of this object as held by the server
	///
	int globalID = 0;

	///
	///Unique ID used to refer to the object
	///
	int ID = 0;

	bool renderable = false;
	std::shared_ptr<RenderableComponent> renderableComponent;
	std::shared_ptr<LocationComponent> locationComponent;
public:
	///Objects are renderable by default
	GameObject();
	GameObject(bool renderable);
	~GameObject();

	std::shared_ptr<LocationComponent> getLocationComponent() { return locationComponent;  }
	std::shared_ptr<RenderableComponent> getRenderableComponent() { return renderableComponent; }
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