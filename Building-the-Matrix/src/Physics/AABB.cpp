#include "../Common.hpp"

#include <glm/vector_relational.hpp>

#include "../ObjectManager.hpp"
#include "../RenderableComponent.hpp"
#include "AABB.hpp"
#include "PhysicsMaths.hpp"

AABB::AABB(const vertexVector vertices)
{
	min = max = glm::vec3();
	if (vertices.size() == 0) {
		return;
	}

	max = min = vertices.at(0);
	for (size_t i = 1; i < vertices.size(); i++){
		// Piecewise: vertex > max
		glm::bvec3 bVLT = glm::greaterThan(vertices.at(i), max);
		// Piecewise: vertex < min
		glm::bvec3 bVMT = glm::lessThan(vertices.at(i), min);
		if (bVLT.x){
			max.x = vertices.at(i).x;
		}
		else if (bVMT.x){
			min.x = vertices.at(i).x;
		}
		if (bVLT.y){
			max.y = vertices.at(i).y;
		}
		else if (bVMT.y){
			min.y = vertices.at(i).y;
		}
		if (bVLT.z){
			max.z = vertices.at(i).z;
		}
		else if (bVMT.z){
			min.z = vertices.at(i).z;
		}
	}
}

AABB::AABB(const GameObjectID gID)
{
	ObjectManager &objMan = ObjectManager::getInstance();
	GameObject gameObj = *(objMan.getObject(gID));
	std::vector<GLfloat> rawVertices = gameObj.getRenderableComponent()->getVertexData();
	const vertexVector vertices = PhysicsMaths::convertGLfloatToVec3(rawVertices);
	min = max = glm::vec3();
	if (vertices.size() == 0) {
		return;
	}

	max = min = vertices.at(0);
	for (size_t i = 1; i < vertices.size(); i++){
		// Piecewise: vertex > max
		glm::bvec3 bVLT = glm::greaterThan(vertices.at(i), max);
		// Piecewise: vertex < min
		glm::bvec3 bVMT = glm::lessThan(vertices.at(i), min);
		if (bVLT.x){
			max.x = vertices.at(i).x;
		}
		else if (bVMT.x){
			min.x = vertices.at(i).x;
		}
		if (bVLT.y){
			max.y = vertices.at(i).y;
		}
		else if (bVMT.y){
			min.y = vertices.at(i).y;
		}
		if (bVLT.z){
			max.z = vertices.at(i).z;
		}
		else if (bVMT.z){
			min.z = vertices.at(i).z;
		}
	}
}

// Destructor not needed.
AABB::~AABB()
{
}

vec3 AABB::getMin() const{
	return min;
}

vec3 AABB::getMax() const{
	return max;
}

vec3 AABB::getCenter() const{
	return (max+min)/2.0f;
}

float AABB::getXSpan() const{
	return max.x-min.x;
}

float AABB::getYSpan() const{
	return max.z - min.z;
}

float AABB::getZSpan() const{
	return max.z - min.z;
}

bool AABB::intersects(const AABB &aabb) const{
	return PhysicsMaths::simpleCollision(*this, aabb);
}

inline bool getBit(char c, char bit){
	return (c >> bit) & 1;
}

// Returns a length 8 vec3 containing
// each corner of the AABB explicitly
std::shared_ptr<vertexVector> AABB::getFullBox() const{
	std::shared_ptr<vertexVector> fullBox = std::make_shared<vertexVector>();
	fullBox->reserve(8);

	// Fill in each vertex
	for (char i = 0; i < 8; i++){
		fullBox->emplace_back(
			getBit(i, 0) ? min.x : max.x,
			getBit(i, 1) ? min.y : max.y,
			getBit(i, 2) ? min.z : max.z
			);
	}
	return fullBox;
}
