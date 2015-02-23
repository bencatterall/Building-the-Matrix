#include "AABB.hpp"
#include "PhysicsMaths.hpp"

AABB::AABB(Serializer serializer, unsigned char *buffer, int &next){
	next += (this->deserialize(serializer, buffer));
}

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

AABB::AABB(const vec3 minimum, const vec3 maximum) :
		min(minimum), max(maximum)
{
}

AABB::AABB(const AABB& other){
	(this->min) = other.min;
	(this->max) = other.max;
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

int AABB::serialize(Serializer serializer, unsigned char *buffer) {
	int next = 0;
	next += serializer.packFloat(&buffer[next], min.x);
	next += serializer.packFloat(&buffer[next], min.y);
	next += serializer.packFloat(&buffer[next], min.z);
	next += serializer.packFloat(&buffer[next], max.x);
	next += serializer.packFloat(&buffer[next], max.y);
	next += serializer.packFloat(&buffer[next], max.z);
	return next;
}

int AABB::deserialize(Serializer serializer, unsigned char *buffer) {
	int next = 0;
	(this->min.x) = serializer.unpackFloat(&buffer[next], next);
	(this->min.y) = serializer.unpackFloat(&buffer[next], next);
	(this->min.z) = serializer.unpackFloat(&buffer[next], next);
	(this->max.x) = serializer.unpackFloat(&buffer[next], next);
	(this->max.y) = serializer.unpackFloat(&buffer[next], next);
	(this->max.z) = serializer.unpackFloat(&buffer[next], next);
	return next;
}