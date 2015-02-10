#include "AABB.hpp"
#include "PhysicsMaths.hpp"

AABB::AABB(const vec3 * vertices, int size)
{
	assert(size > 0);
	max = min = vertices[0];
	for (int i = 1; i < size; i++){
		// Piecewise: vertex > max
		glm::bvec3 bVLT = glm::greaterThan(vertices[i], max);
		// Piecewise: vertex < min
		glm::bvec3 bVMT = glm::lessThan(vertices[i], min);
		if (bVLT.x){
			max.x = vertices[i].x;
		}
		else if (bVMT.x){
			min.x = vertices[i].x;
		}
		if (bVLT.y){
			max.y = vertices[i].y;
		}
		else if (bVMT.y){
			min.y = vertices[i].y;
		}
		if (bVLT.z){
			max.z = vertices[i].z;
		}
		else if (bVMT.z){
			min.z = vertices[i].z;
		}
	}
}

// More sensible constructor - uses vec3 for array
AABB::AABB(const vertexVector vertices){
	// TODO Implement me!
}

#ifdef GAME_MANAGER_H
AABB::AABB(const GameObjectID gID){
	ObjectManager objMan = ObjectManager::getInstance();
	GameObject = objMan.getobj(gID);
	const vec3 *vertices = GameObject.getLocalCoOrds();
	const int vertexCount = GameObject.getVertexCount();
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
std::shared_ptr<vertexVector> AABB::getFullBox() const{
	std::shared_ptr<vertexVector> fullBoxPtr = std::make_shared<vertexVector>();
	fullBoxPtr->resize(8);
	// Fill in each vertex
	for (char i = 0; i < 8; i++){
		fullBoxPtr->emplace_back(
			getBit(i, 0) ? min.x : max.x,
			getBit(i, 1) ? min.y : max.y,
			getBit(i, 2) ? min.z : max.z
			);
	}
	return fullBoxPtr;
}