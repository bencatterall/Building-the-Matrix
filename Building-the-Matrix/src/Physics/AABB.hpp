#ifndef AABB_H
#define AABB_H
#include "../Common.hpp"
#include <memory>
#include <vector>

using glm::vec3;

typedef std::vector<vec3> vertexVector;
typedef int GameObjectID;


class AABB
{
public:
	AABB(const vec3 *, int);
	AABB(const vertexVector);
	AABB(const GameObjectID);
	virtual ~AABB();

	vec3 getMin() const;
	vec3 getMax() const;
	vec3 getCen() const;
	float getXSpan() const;
	float getYSpan() const;
	float getZSpan() const;

	bool intersects(const AABB &) const;
	std::shared_ptr<vertexVector> getFullBox() const;

protected:

private:
	vec3 min, center, max;
	float xSpan, ySpan, zSpan;
};

#endif AABB_H