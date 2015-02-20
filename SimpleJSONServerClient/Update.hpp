#include "CommonMinimal.hpp"
#include "GameObject.hpp"
#include <memory>

class Update {
	private:
		GameObjectGlobalID ID;
		std::shared_ptr<GameObject> object;
	public:
		Update(GameObjectGlobalID ID, std::shared_ptr<GameObject> object);
		Update();
		~Update();
		GameObjectGlobalID getObjectID();
		std::shared_ptr<GameObject> getEditedObject();
};