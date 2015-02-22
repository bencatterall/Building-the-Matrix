#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "../CommonMinimal.hpp"
#include "../../Building-the-Matrix/Dependencies/glm/glm.hpp"
#include "../../Building-the-Matrix/Dependencies/glm/gtc/quaternion.hpp"
#include "../Serializer.hpp"
#include <memory>
#include <vector>

#define TURN_SPEED 0.02f

class AABB;
class LocationComponent;
class Serializer;

using glm::vec3;

typedef std::vector<vec3> vertexVector;
typedef glm::quat Quaternion;

class PhysicsObject
{
public:
	PhysicsObject(Serializer serializer, unsigned char *serial, int& pointer);
	PhysicsObject(std::shared_ptr<LocationComponent>, const vertexVector);
	PhysicsObject(const PhysicsObject& other);
	virtual ~PhysicsObject();
	
	const std::shared_ptr<AABB> getWorldAABB() const;
	const AABB getLocalAABB() const;
	
	const vec3 getX() const;
	const vec3 getV() const;
	const vec3 getA() const;
	const vec3 getOrientation() const;
	void setX(vec3 &);
	void setV(vec3 &);
	void setA(vec3 &);
	void setOrientation(vec3 &);
	
	float getMass() const;
	float getInvMass() const;
	float getRest() const;
	float getQuadDrag() const;
	float getLinDrag() const;
	void setMass(const float);
	void setRest(const float);
	void setQuadDrag(const float);
	void setLinDrag(const float);

	void acceleratePlayer();
	void reversePlayer();
	void turnLeft(float);
	void turnRight(float);

	int serialize(Serializer serializer, unsigned char *buffer);
	int deserialize(Serializer serializer, unsigned char *buffer);
protected:

private:
	void turnObject(const Quaternion rotator, const vec3(PhysicsObject::*getter) () const, void (PhysicsObject::*setter) (vec3 &));

	// Will need some more information later
	std::shared_ptr<AABB> boundingBox;
	float restitution;		// Co-efficent of restitution
	float mass;				// Mass of Object, with 0 representing +inf
	float inverseMass;		// Precomputed value of (1/mass)
	float friction;			// Co-efficient of linear friction
	float airRes;			// Co-efficient of quadratic drag
	std::shared_ptr<LocationComponent> location;
	vec3 velocity;			// Velocity
	vec3 acc;				// Acceleration
	vec3 orientation;

	// TODO implement these
	//Quaternion spin;
	//vec3 angularVelocity;
	//vec3 angulurMomentum;
	//vec3 torque;
	//

};

#endif
