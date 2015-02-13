#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <vector>

class AABB;
class LocationComponent;
class RenderableComponent;

using glm::vec3;

typedef std::vector<vec3> vertexVector;
typedef glm::quat Quaternion;

class PhysicsObject
{
public:
	PhysicsObject(std::shared_ptr<LocationComponent>);
	PhysicsObject(std::shared_ptr<LocationComponent>, std::shared_ptr<RenderableComponent>);
	PhysicsObject(vertexVector);
	PhysicsObject(vertexVector, vec3, float, float);
	PhysicsObject(vertexVector, vec3, vec3, float, float);
	virtual ~PhysicsObject();

	const std::shared_ptr<AABB> getWorldAABB() const;
	const AABB getLocalAABB() const;
	
	const vec3 getX() const;
	const vec3 getV() const;
	const vec3 getA() const;
	void setX(vec3 &);
	void setV(vec3 &);
	void setA(vec3 &);
	
	float getMass() const;
	float getInvMass() const;
	float getRest() const;


protected:

private:
	// Will need some more information later
	vertexVector vertices;
	std::shared_ptr<AABB> boundingBox;
	float restitution; // Co-efficent of restitution
	float mass;		// Mass of Object, with 0 representing +inf
	float inverseMass; // Precomputed value of (1/mass)
	float friction;	// Co-efficient of linear friction
	float airRes;	// Co-efficient of quadratic drag
	vec3 position;	// Centre position
	std::shared_ptr<LocationComponent> location;
	std::shared_ptr<RenderableComponent> rendComp;
	vec3 velocity;	// Velocity
	vec3 acc;		// Acceleration

	// TODO implement these
	Quaternion spin;
	Quaternion orientation;
	vec3 angularVelocity;
	vec3 angulurMomentum;
	vec3 torque;
	//

};

#endif
