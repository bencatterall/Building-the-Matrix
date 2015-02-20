#include "GameObject.hpp"

GameObject::GameObject(GameObjectGlobalID id, bool userObj) {
	(this->ID) = id;
	(this->userControllable) = userObj;
}

GameObject::GameObject(const char *buffer) {
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
}

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::keyPressed(char key) {
	if (this->userControllable) {
		(this->control).regKeyPress(key);
	}
}

void GameObject::setPRY(float pitch, float roll, float yaw) {
	if (this->userControllable) {
		(this->pitch) = pitch;
		(this->roll) = roll;
		(this->yaw) = yaw;
	}
}

void GameObject::keyUnpressed(char key) {
	if (this->userControllable) {
		(this->control).regKeyUnpress(key);
	}
}

GameObjectGlobalID GameObject::getID(){
	return this->ID;
}

virtual int GameObject::serialize(char* buffer) {
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
}
