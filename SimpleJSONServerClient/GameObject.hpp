#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <memory>

#include "Common.hpp"
#include "Controls.hpp"
#include "LocationComponent.hpp"
#include "Physics\PhysicsObject.hpp"

class GameObject {
	public:
		GameObjectGlobalID ID;
		//add in position as a vector etc...
		std::shared_ptr<LocationComponent> locComp;
		std::shared_ptr<PhysicsObject> physComp;
		float xrot;
		float yrot;
		float zrot;
		float xpos;
		float ypos;
		float zpos;
		bool visible;
		bool renderable;
		bool deleted;
		//for use with user vehicles only
		bool userControllable;
	public:
		GameObject(GameObjectGlobalID id);
		GameObject(const char *buffer); // for unserialization
		void keyPressed(char key);
		void keyUnpressed(char key);
		GameObject(const GameObject& other);
		GameObject();
		virtual ~GameObject();
		GameObjectGlobalID getID();
		virtual int serialize(char* buffer);		
};
#endif