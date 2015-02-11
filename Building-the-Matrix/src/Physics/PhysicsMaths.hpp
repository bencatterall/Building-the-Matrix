#ifndef PHYSICS_MATHS_H
#define PHYSICS_MATHS_H
#include "../Common.hpp"

#include <algorithm>
#include <glm/vec3.hpp>
#include <vector>

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

	std::vector<glm::vec3> convertGLfloatToVec3(std::vector<GLfloat> data);
};

#endif