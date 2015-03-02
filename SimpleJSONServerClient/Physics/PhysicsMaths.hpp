#ifndef PHYSICS_MATHS_H
#define PHYSICS_MATHS_H

#include <vector>
#include <memory>
#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#include "../../Building-the-Matrix/Dependencies/glm/gtc/quaternion.hpp"
#include "../../Building-the-Matrix/Dependencies/glm/gtc/matrix_transform.hpp"
#include "../CommonMinimal.hpp"
#include "PhysicsObject.hpp"

using glm::vec3;

// Saves including a large library.
typedef float GLfloat;

class AABB;
class GameObject;

typedef std::vector<vec3> vertexVector;
typedef glm::quat Quaternion;

namespace PhysicsMaths
{
	bool simpleCollision(const AABB &, const AABB &);
	bool simpleCollision(const GameObjectGlobalID, const GameObjectGlobalID);

	void handleCollision(GameObjectGlobalID, GameObjectGlobalID, std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> &);
	void handleCollision(GameObject &, GameObject &);

	std::vector<vec3> convertGLfloatToVec3(std::vector<GLfloat> &);

	vec3 translateVertex(const glm::mat4x4 &, const vec3 &);
	std::shared_ptr<vertexVector> translateVertexVector(const glm::mat4x4 &, const std::shared_ptr<vertexVector> &);
};

#endif
