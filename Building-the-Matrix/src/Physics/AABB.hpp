#ifndef AABB_H
#define AABB_H

#include "../Common.hpp"
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

using glm::vec3;

typedef std::vector<vec3> vertexVector;

class AABB
{
public:
	AABB(const vec3, const vec3);
	AABB(unsigned char *buffer, int & next){
		next += (this->deserialize(buffer));
	}
	virtual ~AABB();
	int deserialize(unsigned char *buffer);

private:
	vec3 min, max;
};

#endif AABB_H