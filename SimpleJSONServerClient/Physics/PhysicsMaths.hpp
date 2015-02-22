#ifndef PHYSICS_MATHS_H
#define PHYSICS_MATHS_H

#include "../Common.hpp"
#include "../../Building-the-Matrix/Dependencies/glew/glew.h"
#include "../../Building-the-Matrix/Dependencies/GLFW/glfw3.h"
#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

#include "PhysicsObject.hpp"

#define PI 3.14159265358979323846f

using glm::vec3;

class AABB;
class GameObject;

typedef std::vector<vec3> vertexVector;
typedef glm::quat Quaternion;

namespace PhysicsMaths
{
	float UATtoS(float, float, float);
	float UATtoV(float, float, float);

	vec3 UATtoS(const vec3, const vec3, float);
	vec3 UATtoV(const vec3, const vec3, float);

	bool simpleCollision(const AABB &, const AABB &);
	bool simpleCollision(const PhysicsObject, const PhysicsObject);
	bool simpleCollision(const GameObjectGlobalID a, const GameObjectGlobalID b);
	bool complexCollision(const GameObjectGlobalID a, const GameObjectGlobalID b);
	bool sat(const vec3 &, std::shared_ptr<PhysicsObject>, std::shared_ptr<PhysicsObject>, std::shared_ptr<vertexVector>, std::shared_ptr<vertexVector>);

	void handleCollision(GameObjectGlobalID, GameObjectGlobalID);
	void handleCollision(GameObject &, GameObject &);
	void stepObject(PhysicsObject &, float);

	std::vector<vec3> convertGLfloatToVec3(std::vector<GLfloat> data);
	const vec3 translateVertex(const glm::mat4x4 &, const vec3);
	const std::shared_ptr<vertexVector> translateVertexVector(const glm::mat4x4 &, const std::shared_ptr<vertexVector>);

	void acceleratePlayer(const GameObjectGlobalID);
	void reversePlayer(const GameObjectGlobalID);

	void turnLeft(const GameObjectGlobalID, float = TURN_SPEED);
	void turnRight(const GameObjectGlobalID, float = TURN_SPEED);
	void turnObject(std::shared_ptr<PhysicsObject> phys, Quaternion rotator, const vec3 (PhysicsObject::*getter) () const, void (PhysicsObject::*setter) (vec3 &));

	vec3 convertYPRtoDirection(const vec3);
	vec3 convertDirectiontoYPR(const vec3);
};

#endif