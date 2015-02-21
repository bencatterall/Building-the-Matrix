#include "GameObject.hpp"

struct Vector{
	float x, y, z;
};

GameObject::GameObject(GameObjectGlobalID id) {
	(this->ID) = id;
	(this->userControllable) = false;
	(this->deleted) = false;
	this->locComp = std::make_shared<LocationComponent>();
	std::vector<vec3> tmpVec = std::vector<vec3>();
	tmpVec.emplace_back(-4.0f, -4.0f, -4.0f);
	tmpVec.emplace_back(4.0f, 4.0f, 4.0f);
	this->physComp = std::make_shared<PhysicsObject>(locComp, tmpVec);
}

GameObject::GameObject(const GameObject& other) {
	//copy
	(this->ID) = other.ID;
	(this->xrot) = other.xrot;
	(this->yrot) = other.yrot;
	(this->zrot) = other.zrot;
	(this->xpos) = other.xpos;
	(this->ypos) = other.ypos;
	(this->zpos) = other.zpos;
	(this->visible) = other.visible;
	(this->renderable) = other.renderable;
	(this->deleted) = other.deleted;
	(this->userControllable) = other.userControllable;
}

/* GameObject::GameObject(const char *buffer) {
	struct SerializedObject {
		float xrot;
		float yrot;
		float zrot;
		float xpos;
		float ypos;
		float zpos;
		bool visible;
		bool renderable;
	} obj;
	memcpy(obj, buffer, sizeof(obj));
	xrot = obj.xrot;
	xrot = obj.yrot;
	zrot = obj.zrot;
	xpos = obj.xpos;
	ypos = obj.ypos;
	zpos = obj.zpos;
	visible = obj.visible;
	renderable = obj.renderable;
} */

GameObject::GameObject() {}

GameObject::~GameObject() {}

GameObjectGlobalID GameObject::getID(){
	return this->ID;
}

int GameObject::serialize(char* buffer) {
	struct SerializedObject {
		float xrot;
		float yrot;
		float zrot;
		float xpos;
		float ypos;
		float zpos;
		/*
		Vector orientation;
		Vector position;
		Vector velocity;
		Vector acceleration;
		*/
		bool visible;
		bool renderable;
	} obj;
	
	obj.xrot = xrot;
	obj.yrot = xrot;
	obj.zrot = zrot;
	obj.xpos = xpos;
	obj.ypos = ypos;
	obj.zpos = zpos;

	/*
	serializeVec3(physComp->getX(), obj.position);
	serializeVec3(physComp->getV(), obj.velocity);
	serializeVec3(physComp->getA(), obj.acceleration);
	serializeVec3(physComp->getOrientation(), obj.orientation);
	*/

	obj.visible = visible;
	obj.renderable = renderable;
	// TODO: Check me
	const void * objPointer = &obj;
	//
	memcpy(buffer, objPointer, sizeof(obj));
	return sizeof(obj);
}

void serializeVec3(const vec3 & input, Vector & output){
	output.x = input.x;
	output.y = input.y;
	output.z = input.z;
}