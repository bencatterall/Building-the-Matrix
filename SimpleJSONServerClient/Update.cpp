#include "Update.hpp"

Update::Update(GameObjectGlobalID ID, std::shared_ptr<GameObject> object){
	(this->ID) = ID;
	(this->object) = object;
}

Update::Update(){}

Update::~Update(){}

GameObjectGlobalID Update::getObjectID() {
	return (this->ID);
}

std::shared_ptr<GameObject> Update::getEditedObject() {
	return (this->object);
}
