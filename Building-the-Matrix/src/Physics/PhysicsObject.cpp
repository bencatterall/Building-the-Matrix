#include "../LocationComponent.hpp"
#include "AABB.hpp"
#include "PhysicsObject.hpp"
#include "../Serializer.hpp"

PhysicsObject::PhysicsObject(std::shared_ptr<LocationComponent> locationComp)
	: restitution(1.0f), mass(1.0f), inverseMass(1.0f),
	  velocity(vec3())
{
	this->location = locationComp;
	boundingBox = std::make_shared<AABB>(vec3(),vec3());
}

PhysicsObject::~PhysicsObject()
{
}

const vec3 PhysicsObject::getOrientation() const{
	return orientation;
}

int PhysicsObject::deserialize(unsigned char * buffer) {
	Serializer serializer = Serializer();
	int next = 0;
	(this->boundingBox) = std::make_shared<AABB>(&buffer[next], next);
	(this->restitution) = serializer.unpackFloat(&buffer[next], next);
	(this->mass) = serializer.unpackFloat(&buffer[next], next);
	(this->inverseMass) = serializer.unpackFloat(&buffer[next], next);
	(this->friction) = serializer.unpackFloat(&buffer[next], next);
	(this->airRes) = serializer.unpackFloat(&buffer[next], next);
	(this->location) = std::make_shared<LocationComponent>(&buffer[next], next);
	(this->velocity.x) = serializer.unpackFloat(&buffer[next], next);
	(this->velocity.y) = serializer.unpackFloat(&buffer[next], next);
	(this->velocity.z) = serializer.unpackFloat(&buffer[next], next);
	(this->acc.x) = serializer.unpackFloat(&buffer[next], next);
	(this->acc.y) = serializer.unpackFloat(&buffer[next], next);
	(this->acc.z) = serializer.unpackFloat(&buffer[next], next);
	(this->orientation.x) = serializer.unpackFloat(&buffer[next], next);
	(this->orientation.y) = serializer.unpackFloat(&buffer[next], next);
	(this->orientation.z) = serializer.unpackFloat(&buffer[next], next);
	return next;
}