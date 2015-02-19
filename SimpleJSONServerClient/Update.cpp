#include "Update.hpp"

Update::Update(GameObjectGlobalID ID, GameObject object){
	(this->ID) = ID;
	(this->object) = object;
	(this->type) = 0;
}

Update::Update(GameObjectGlobalID ID, GameObject object, int type){
	(this->ID) = ID;
	(this->object) = object;
	(this->type) = type;
}

Update::Update(){}

Update::~Update(){}

GameObjectGlobalID Update::getObjectID() {
	return (this->ID);
}

GameObject Update::getEditedObject() {
	return (this->object);
}

bool Update::forDeletion() {
	return ((this->type) == 1);
}
