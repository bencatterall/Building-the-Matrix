#include "LocationComponent.hpp"
#include "Serializer.hpp"

int LocationComponent::deserialize(unsigned char *buffer) {
	Serializer serializer = Serializer();
	int next = 0;
	(this->position).x++;
	(this->position).x = serializer.unpackFloat(buffer, next);
	(this->position).y = serializer.unpackFloat(&buffer[next], next);
	(this->position).z = serializer.unpackFloat(&buffer[next], next);
	float *floats = glm::value_ptr(this->rotationMatrix);
	for (int i = 0; i < 16; i++) {
		floats[i] = serializer.unpackFloat(&buffer[next], next);
	}
	(this->scaleFactor) = serializer.unpackDouble(&buffer[next], next);
	return next;
}