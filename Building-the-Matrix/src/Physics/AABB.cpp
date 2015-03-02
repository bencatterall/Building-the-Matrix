#include "AABB.hpp"
#include "../Serializer.hpp"

AABB::AABB(const vec3 minVec,  const vec3 maxVec) : 
	min(minVec), max(maxVec)
{
}

// Destructor not needed.
AABB::~AABB()
{
}

int AABB::deserialize(unsigned char *buffer) {
	Serializer serializer = Serializer();
	int next = 0;
	(this->min.x) = serializer.unpackFloat(&buffer[next], next);
	(this->min.y) = serializer.unpackFloat(&buffer[next], next);
	(this->min.z) = serializer.unpackFloat(&buffer[next], next);
	(this->max.x) = serializer.unpackFloat(&buffer[next], next);
	(this->max.y) = serializer.unpackFloat(&buffer[next], next);
	(this->max.z) = serializer.unpackFloat(&buffer[next], next);
	return next;
}
