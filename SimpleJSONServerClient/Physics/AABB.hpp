#ifndef AABB_H
#define AABB_H

#include "../Common.hpp"
#include "../../Building-the-Matrix/Dependencies/glm/glm.hpp"
#include "../../Building-the-Matrix/Dependencies/glm/vector_relational.hpp"
#include "../Serializer.hpp"
#include <memory>
#include <vector>

class Serializer;

using glm::vec3;

typedef std::vector<vec3> vertexVector;


class AABB
{
public:
	AABB() {}
	AABB(const vertexVector);
	AABB(Serializer serializer, unsigned char *buffer, int &next);
	AABB(const vec3, const vec3);
	AABB(const GameObjectGlobalID);
	AABB(const AABB& other);
	virtual ~AABB();

	vec3 getMin() const;
	vec3 getMax() const;
	vec3 getCenter() const;
	float getXSpan() const;
	float getYSpan() const;
	float getZSpan() const;

	bool intersects(const AABB &) const;
	std::shared_ptr <vertexVector> getFullBox() const;

	int serialize(Serializer serializer, unsigned char *buffer);
	int deserialize(Serializer serializer, unsigned char *buffer);

protected:

private:
	vec3 min, max;
};

#endif AABB_H