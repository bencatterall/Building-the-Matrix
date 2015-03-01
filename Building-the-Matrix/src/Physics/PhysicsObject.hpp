#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "../src/Common.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <vector>

#define TURN_SPEED 0.02f

class AABB;
class LocationComponent;

using glm::vec3;

typedef std::vector<vec3> vertexVector;
typedef glm::quat Quaternion;

class PhysicsObject
{
public:
	PhysicsObject(std::shared_ptr<LocationComponent>);
	PhysicsObject(unsigned char *serial, int& pointer) {
		pointer += (this->deserialize(serial));
	}
	virtual ~PhysicsObject();
	int deserialize(unsigned char * buffer);
	std::shared_ptr<LocationComponent> getLocationComponent() {
		return this->location;
	}
	const vec3 getOrientation() const;

private:
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
};

#endif
