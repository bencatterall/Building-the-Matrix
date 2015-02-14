#ifndef PHYSICS_MATHS_H
#define PHYSICS_MATHS_H


#include "../Common.hpp"

#include <algorithm>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <memory>

#define GRAVITY 9.81

using glm::vec3;

class AABB;
class PhysicsObject;

typedef std::vector<vec3> vertexVector;

namespace PhysicsMaths
{
	float UATtoS(float, float, float);
	float UATtoV(float, float, float);

	vec3 UATtoS(const vec3, const vec3, float);
	vec3 UATtoV(const vec3, const vec3, float);

	bool simpleCollision(const AABB &, const AABB &);
	bool simpleCollision(const PhysicsObject, const PhysicsObject);

	void handleCollision(GameObjectID, GameObjectID);
	void stepObject(PhysicsObject, float);

	std::vector<vec3> convertGLfloatToVec3(std::vector<GLfloat> data);
	const vec3 translateVertex(const glm::mat4x4, const vec3);
	const std::shared_ptr<vertexVector> translateVertexVector(const glm::mat4x4, const std::shared_ptr<vertexVector>);
};

#endif