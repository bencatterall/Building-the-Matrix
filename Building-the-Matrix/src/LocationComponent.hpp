#ifndef LOCATION_COMPONENT
#define LOCATION_COMPONENT

#include "Common.hpp"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	LocationComponent() :
		LocationComponent(glm::vec3(0.0, 0.0, 0.0), glm::mat4(1.0)) {
	}

	LocationComponent::LocationComponent(unsigned char *buffer, int &next){
		next += (this->deserialize(buffer));
	}

	LocationComponent(glm::vec3 pos, glm::mat4 rotationMat, double sf = 1.0) :
		position(pos), rotationMatrix(rotationMat),
		scaleFactor(sf)
	{

	}

	int deserialize(unsigned char *buffer);

	///
	/// Get and set position
	///
	void setPosition(glm::vec3 pos) { position = pos; }
	glm::vec3 getPosition() { return position; }

	///
	/// Get and set rotation matrix
	///
	void setRotationMatrix(glm::mat4 rotationMat) { rotationMatrix = rotationMat; }
	glm::mat4 getRotationMatrix() { return rotationMatrix; }

	///
	/// Get and set scale factor
	///
	void setScaleFactor(double sf) { scaleFactor = sf; }
	double getScaleFactor() { return scaleFactor; } 


};

#endif