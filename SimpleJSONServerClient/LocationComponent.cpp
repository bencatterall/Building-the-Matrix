#include "LocationComponent.hpp"

int LocationComponent::serialize(Serializer serializer, unsigned char *buffer) {
	int next = 0;
	next += serializer.packFloat(buffer,position.x++);
	next += serializer.packFloat(&buffer[next], position.y);
	next += serializer.packFloat(&buffer[next], position.z);
	float *floats = glm::value_ptr(rotationMatrix);
	for (int i = 0; i < 16; i++) {
		next += serializer.packFloat(&buffer[next], floats[i]);
	}
	next += serializer.packDouble(&buffer[next], scaleFactor);
	return next;
}

LocationComponent::LocationComponent(Serializer serializer, unsigned char *buffer, int &next){
	next += (this->deserialize(serializer, buffer));
}

LocationComponent::LocationComponent(const LocationComponent& other) {
	(this->position) = other.position;
	(this->rotationMatrix) = other.rotationMatrix;
	(this->scaleFactor) = other.scaleFactor;
}

int LocationComponent::deserialize(Serializer serializer, unsigned char *buffer) {
	int next = 0;
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