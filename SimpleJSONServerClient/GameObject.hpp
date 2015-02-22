#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <memory>

#include "CommonMinimal.hpp"
#include "Controls.hpp"
#include "LocationComponent.hpp"
#include "Physics\PhysicsObject.hpp"

class GameObject {
	public:
		GameObjectGlobalID ID;
		//add in position as a vector etc...
		std::shared_ptr<LocationComponent> locComp;
		std::shared_ptr<PhysicsObject> physComp;
		bool visible;
		bool renderable;
		bool deleted;
		//for use with user vehicles only
		bool userControllable;
	public:
		GameObject(GameObjectGlobalID id);
		GameObject(unsigned char *buffer, int &size); // for unserialization
		void keyPressed(char key);
		void keyUnpressed(char key);
		GameObject(const GameObject& other);
		GameObject();
		virtual ~GameObject();
		GameObjectGlobalID getID();
		int serialize(unsigned char* buffer);
		int deserialize(unsigned char* buffer);
};
#endif