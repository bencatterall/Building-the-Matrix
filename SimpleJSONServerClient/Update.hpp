#include "CommonMinimal.hpp"
#include "GameObject.hpp"

class Update {
	private:
		GameObjectGlobalID ID;
		GameObject object;
		//0 for edit, 1 for deletion
		int type;
	public:
		Update(GameObjectGlobalID ID, GameObject object);
		Update(GameObjectGlobalID ID, GameObject object,int type);
		Update();
		~Update();
		GameObjectGlobalID getObjectID();
		GameObject getEditedObject();
		bool forDeletion();
};