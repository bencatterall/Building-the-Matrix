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
	AABB(const vertexVector);
	AABB(const vec3, const vec3);
	AABB(const GameObjectGlobalID);
	virtual ~AABB();

	vec3 getMin() const;
	vec3 getMax() const;
	vec3 getCenter() const;
	float getXSpan() const;
	float getYSpan() const;
	float getZSpan() const;

	bool intersects(const AABB &) const;
	std::shared_ptr <vertexVector> getFullBox() const;

protected:

private:
	vec3 min, max;
};

#endif AABB_H