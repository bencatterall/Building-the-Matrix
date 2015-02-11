#include "AABB.hpp"
#include "PhysicsMaths.hpp"
#include "../ObjectManager.hpp"
#include "../RenderableComponent.hpp"


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

#ifdef FIX
AABB::AABB(const GameObjectID gID)
{
	ObjectManager objMan = ObjectManager::getInstance();
	GameObject gameObj = *(objMan.getObject(gID));
	const vec3 *vertices = gameObj.getRenderableComponent()->getVertexData();
	const int vertexCount = gameObj.getRenderableComponent()->getVertexDataSize();
	AABB(vertices, vertexCount); // TODO Check if this is valid
}
#endif


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

vec3 AABB::getCen() const{
	return center;
}

float AABB::getXSpan() const{
	return xSpan;
}

float AABB::getYSpan() const{
	return ySpan;
}

float AABB::getZSpan() const{
	return zSpan;
}

bool AABB::intersects(const AABB &aabb) const{
	return PhysicsMaths::simpleCollision(*this, aabb);
}

inline bool getBit(char c, char bit){
	return (c >> bit) & 1;
}

// Returns a length 8 vec3 containing
// each corner of the AABB explicitly
vertexVector AABB::getFullBox() const{
	vertexVector fullBox;
	fullBox.resize(8);

	// Fill in each vertex
	for (char i = 0; i < 8; i++){
		fullBox.emplace_back(
			getBit(i, 0) ? min.x : max.x,
			getBit(i, 1) ? min.y : max.y,
			getBit(i, 2) ? min.z : max.z
			);
	}
	return fullBox;
}
