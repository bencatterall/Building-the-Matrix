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
<<<<<<< HEAD

		GameObject(GameObjectGlobalID id);
		GameObject(const GameObject& other);
=======
		Control control;
		float pitch;
		float roll;
		float yaw;
	public:
		GameObject(GameObjectGlobalID id, bool userObj);
		GameObject(const char *buffer); // for unserialization
		void keyPressed(char key);
		void keyUnpressed(char key);
>>>>>>> b0d51678cb0e811dfa152d3d5dc72d931e99be57
		GameObject();
		virtual ~GameObject();
		GameObjectGlobalID getID();
<<<<<<< HEAD
=======
		void setPRY(float pitch, float roll, float yaw);
		virtual int serialize(char* buffer);		
>>>>>>> b0d51678cb0e811dfa152d3d5dc72d931e99be57
};
#endif