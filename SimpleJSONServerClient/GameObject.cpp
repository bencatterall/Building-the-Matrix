#include "GameObject.hpp"

GameObject::GameObject(GameObjectGlobalID id) {
	(this->ID) = id;
	(this->userControllable) = false;
	(this->deleted) = false;
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

/* virtual int GameObject::serialize(char* buffer) {
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
	obj.xrot = xrot;
	obj.yrot = xrot;
	obj.zrot = zrot;
	obj.xpos = xpos;
	obj.ypos = ypos;
	obj.zpos = zpos;
	obj.visible = visible;
	obj.renderable = renderable;

	memcpy(buffer, obj, sizeof(obj));
	return sizeof(obj);
} */
