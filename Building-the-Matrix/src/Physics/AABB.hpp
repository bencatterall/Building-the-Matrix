#ifndef AABB_H
#define AABB_H

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "../CommonMinimal.hpp"

using glm::vec3;

typedef std::vector<vec3> vertexVector;


class AABB
{
public:
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
	std::shared_ptr <vertexVector> getFullBox() const;

protected:

private:
	vec3 min, center, max;
	float xSpan, ySpan, zSpan;
};

#endif AABB_H