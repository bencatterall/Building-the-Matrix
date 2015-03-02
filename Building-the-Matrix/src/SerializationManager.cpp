#include "SerializationManager.hpp"
#include "Serializer.hpp"

#include <type_traits> //for std::underlying_type

std::vector<char> SerializationManager::buildLoginMessage() {
	MessageType type = MessageType::LOGIN;
	int value = static_cast<std::underlying_type<MessageType>::type>(type);

	std::vector<char> data;
	return data;
}

std::vector<char> SerializationManager::buildLogoutMessage() {
	std::vector<char> data;
	return data;
}

std::vector<char> buildStateChangeMessage() {
	std::vector<char> data;
	return data;
}


std::vector<char> buildObjectDeletedMessage() {
	std::vector<char> data;
	return data;
}