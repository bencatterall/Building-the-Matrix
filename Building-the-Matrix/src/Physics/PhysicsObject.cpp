#include "../LocationComponent.hpp"
#include "../RenderableComponent.hpp"
#include "AABB.hpp"
#include "PhysicsObject.hpp"
#include "PhysicsMaths.hpp"


PhysicsObject::PhysicsObject(std::shared_ptr<LocationComponent> locationComp, std::shared_ptr<RenderableComponent> rendComp)
	: mass(1.0f), inverseMass(1.0f),
	restitution(1.0f), vertices(vertices),
	velocity(vec3())
{
	this->location = locationComp;
	this->rendComp = rendComp;
	boundingBox = std::make_shared<AABB>(PhysicsMaths::convertGLfloatToVec3(rendComp->getVertexData()));
}

PhysicsObject::~PhysicsObject()
{
}

const std::shared_ptr<AABB> PhysicsObject::getWorldAABB() const {
	glm::mat4x4 projMat = rendComp->getProjectionMatrix();
	std::shared_ptr<vertexVector> fullBox = boundingBox->getFullBox();
	std::shared_ptr<vertexVector> worldSpace = PhysicsMaths::translateVertexVector(projMat, fullBox);
	std::shared_ptr<AABB> worldBox = std::make_shared<AABB>(*worldSpace);
	return worldBox;
}

// TODO: Refactor me
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
	location->setPosition(newPos);
}

void PhysicsObject::setV(vec3 &newVelocity){
	velocity = vec3(newVelocity);
}

void PhysicsObject::setA(vec3 &newAcc){
	acc = vec3(newAcc);
}

const vec3 PhysicsObject::getX() const{
	return location->getPosition();
}

const vec3 PhysicsObject::getV() const{
	return velocity;
}

const vec3 PhysicsObject::getA() const{
	return acc;
}

float PhysicsObject::getLinDrag() const{
	return friction;
}

float PhysicsObject::getQuadDrag() const{
	return airRes;
}

const vec3 PhysicsObject::getOrientation() const{
	return orientation;
}

void PhysicsObject::setOrientation(vec3 & v){
	orientation = glm::normalize(v);
}

void PhysicsObject::setMass(const float mass){
	this->mass = mass;
	if (mass == 0.0f){
		inverseMass = 0.0f;
	}
	else{
		inverseMass = 1.0f / mass;
	}
}

void PhysicsObject::setRest(const float rest){
	restitution = rest;
}

void PhysicsObject::setQuadDrag(const float quadDrag){
	airRes = quadDrag;
}

void PhysicsObject::setLinDrag(const float linDrag){
	friction = linDrag;
}
