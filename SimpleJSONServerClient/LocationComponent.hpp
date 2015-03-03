#ifndef LOCATION_COMPONENT
#define LOCATION_COMPONENT

#include "Common.hpp"
#include "Serializer.hpp"
#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#include "../Building-the-Matrix/Dependencies/glm/vec3.hpp"
#include "../Building-the-Matrix/Dependencies/glm/mat4x4.hpp"
#include "../Building-the-Matrix/Dependencies/glm/gtc/type_ptr.hpp"

class Serializer;

class LocationComponent {

	///
	/// The world position of the object
	///
	glm::vec3 position;

	///
	/// The rotation matrix, holding combined effect of all rotations to be 
	/// applied to the object
	///
	glm::mat4 rotationMatrix;

	///
	/// Object scale factor
	///
	double scaleFactor = 1.0;

public:

	int serialize(Serializer serializer, unsigned char *buffer);

	int deserialize(Serializer serializer, unsigned char *buffer);

	LocationComponent(glm::vec3 pos = glm::vec3(0.0f,0.0f,0.0f), const glm::mat4x4 rotationMat = glm::mat4x4(1.0f), double sf = 1.0) :
		position(pos), rotationMatrix(rotationMat),
		scaleFactor(sf)
	{

	}

	LocationComponent(const LocationComponent& other);
	LocationComponent(Serializer serializer, unsigned char *buffer, int &next);

	///
	/// Get and set position
	///
	void setPosition(glm::vec3 pos) { position = pos; }
	glm::vec3 getPosition() { return position; }

	///
	/// Get and set rotation matrix
	///
	void setRotationMatrix(const glm::mat4 & rotationMat) { rotationMatrix = rotationMat; }
	glm::mat4 getRotationMatrix() { return rotationMatrix; }

	///
	/// Get and set scale factor
	///
	void setScaleFactor(double sf) { scaleFactor = sf; }
	double getScaleFactor() { return scaleFactor; } 
};

#endif