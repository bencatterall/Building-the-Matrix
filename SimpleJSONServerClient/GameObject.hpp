#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "CommonMinimal.hpp"
#include "Controls.hpp"

class GameObject {
	private:
		GameObjectGlobalID ID;
		//add in position as a vector etc...

		//for use with user vehicles only
		bool userControllable;
		Control control;
	public:
		GameObject(GameObjectGlobalID id,bool userObj);
		void keyPressed(char key);
		void keyUnpressed(char key);
		GameObject();
		~GameObject();
		GameObjectGlobalID getID();
};
#endif