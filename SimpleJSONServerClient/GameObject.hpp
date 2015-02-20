#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "CommonMinimal.hpp"
#include "Controls.hpp"

class GameObject {
	private:
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
		//for use with user vehicles only
		bool userControllable;
		Control control;
		float pitch;
		float roll;
		float yaw;
	public:
		GameObject(GameObjectGlobalID id, bool userObj);
		GameObject(const char *buffer); // for unserialization
		void keyPressed(char key);
		void keyUnpressed(char key);
		GameObject();
		~GameObject();
		GameObjectGlobalID getID();
		void setPRY(float pitch, float roll, float yaw);
		virtual int serialize(char* buffer);		
};
#endif