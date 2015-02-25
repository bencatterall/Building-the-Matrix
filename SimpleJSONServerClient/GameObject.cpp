#include "GameObject.hpp"
#include "Serializer.hpp"

GameObject::GameObject(GameObjectGlobalID id) {
	(this->ID) = id;
	(this->userControllable) = false;
	(this->renderable) = true;
	(this->visible) = true;
	(this->deleted) = false;
	this->locComp = std::make_shared<LocationComponent>();
	std::vector<vec3> tmpVec = std::vector<vec3>();
	tmpVec.emplace_back(-10.0f, -10.0f, -10.0f);
	tmpVec.emplace_back(10.0f, 10.0f, 10.0f);
	this->physComp = std::make_shared<PhysicsObject>(locComp, tmpVec);
}

GameObject::GameObject(const GameObject& other) {
	//copy
	(this->ID) = other.ID;
	(this->locComp) = other.locComp;
	(this->physComp) = other.physComp;
	(this->visible) = other.visible;
	(this->renderable) = other.renderable;
	(this->deleted) = other.deleted;
	(this->userControllable) = other.userControllable;
}

GameObject::GameObject(unsigned char *buffer, int &size) {
	size += (this->deserialize(buffer));
} 

GameObject::GameObject() {}

GameObject::~GameObject() {}

GameObjectGlobalID GameObject::getID(){
	return this->ID;
}

//places serialized object in buffer and return the size as an int
int GameObject::serialize(unsigned char* buffer) {
	Serializer serializer = Serializer();
	int next;
	next = serializer.packInt(buffer,(this->ID));
	next += (this->physComp)->serialize(serializer, &(buffer[next]));
	next += serializer.packBool(&buffer[next], visible);
	next += serializer.packBool(&buffer[next], renderable);
	return next;
}

//places serialized values into object return the size of the buffer read as an int 
int GameObject::deserialize(unsigned char* buffer) {
	Serializer serializer = Serializer();
	int next = 0;
	(this->ID) = serializer.unpackInt(buffer,next);
	(this->physComp) = std::make_shared<PhysicsObject>(serializer, &buffer[next], next);
	(this->visible) = serializer.unpackBool(&buffer[next], next);
	(this->renderable) = serializer.unpackBool(&buffer[next], next);
	return next;
}