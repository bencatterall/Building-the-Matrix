#ifndef PHYSICS_MATHS_H
#define PHYSICS_MATHS_H

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#define GRAVITY 9.81

using glm::vec3;

class AABB;
class PhysicsObject;


namespace PhysicsMaths
{
	float UATtoS(float, float, float);
	float UATtoV(float, float, float);

	vec3 UATtoS(const vec3, const vec3, float);
	vec3 UATtoV(const vec3, const vec3, float);

	bool simpleCollision(const AABB &, const AABB &);
	bool simpleCollision(const PhysicsObject &, PhysicsObject &);

	void handleCollision(PhysicsObject &, PhysicsObject &);
	void stepObject(PhysicsObject, float);
};

#endif