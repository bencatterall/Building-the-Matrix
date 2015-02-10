#include "AABB.hpp"
#include "PhysicsObject.hpp"

PhysicsObject::PhysicsObject()
	: mass(1), inverseMass(1),
	restitution(1), vertices(vertices),
	velocity(vec3()), position(vec3()),
	boundingBox(new AABB(std::make_shared<vertexVector>()))
{
}

PhysicsObject::PhysicsObject(vertexVector vertices)
	: mass(1), inverseMass(1),
	restitution(1), vertices(vertices),
	boundingBox(new AABB(vertices)), velocity(vec3()),
	position(vec3())
{
}

PhysicsObject::PhysicsObject(vertexVector vertices, vec3 pos, float mass, float rest)
	: mass(mass), inverseMass(mass ? 1 / mass : 0),
	restitution(rest), vertices(vertices),
	boundingBox(new AABB(vertices)), velocity(vec3()),
	position(vec3())
{
}

PhysicsObject::PhysicsObject(vertexVector vertices, vec3 pos, vec3 velocity, float mass, float rest)
	: mass(mass), inverseMass(mass ? 1 / mass : 0),
	restitution(rest), vertices(vertices),
	boundingBox(new AABB(vertices)), velocity(velocity),
	position(pos)
{
}

// Destructor
PhysicsObject::~PhysicsObject()
{
	delete boundingBox;
}

const std::shared_ptr<AABB> PhysicsObject::getWorldAABB() const {
	vertexVector boundBox = *(boundingBox->getFullBox());
	// TODO: Transform from local to world space
	std::shared_ptr<AABB> worldBox = std::make_shared<AABB>(boundBox);
	return worldBox;
}

const AABB PhysicsObject::getLocalAABB() const {
	return *boundingBox;
}

float PhysicsObject::getMass() const{
	return mass;
}

float PhysicsObject::getInvMass() const{
	return inverseMass;
}

float PhysicsObject::getRest() const{
	return restitution;
}

void PhysicsObject::setX(vec3 &newPos){
	position = vec3(newPos);
}

void PhysicsObject::setV(vec3 &newVelocity){
	velocity = vec3(newVelocity);
}

void PhysicsObject::setA(vec3 &newAcc){
	acc = vec3(newAcc);
}

const vec3 PhysicsObject::getX() const{
	return position;
}

const vec3 PhysicsObject::getV() const{
	return velocity;
}

const vec3 PhysicsObject::getA() const{
	return acc;
}
