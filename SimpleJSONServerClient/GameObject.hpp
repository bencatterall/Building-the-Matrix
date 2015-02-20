#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "CommonMinimal.hpp"
#include "Controls.hpp"

class GameObject {
	public:
		GameObjectGlobalID ID;
		//add in position as a vector etc...
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

		GameObject(GameObjectGlobalID id);
		GameObject(const GameObject& other);
		GameObject();
		virtual ~GameObject();
		GameObjectGlobalID getID();
};
#endif