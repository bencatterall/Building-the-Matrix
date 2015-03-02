#include "Message.hpp" 
#include <iostream>

Message::~Message() {
	delete[] message;
}

int Message::getMessageSize() {
	return (this->messageSize);
}

Message::Message(Address client, unsigned char *message, int length){
	(this->client) = client;
	(this->message) = message;
	(this->messageSize) = length;
}

Message::Message(Address client, std::string event) {
	//std::cout << "event was " << event << "\n";
	if (event.compare("A FELLOW PLAYER HAS LEFT THE GAME")==0){
		//std::cout << "sending A FELLOW PLAYER HAS LEFT THE GAME\n";
		(this->client) = client;
		(this->message) = (unsigned char *)"A FELLOW PLAYER HAS LEFT THE GAME\0";
		(this->messageSize) = 38;
	}
}

Address Message::getClient() {
	return (this->client);
}

unsigned char* Message::getMessage() {
	return (this->message);
}

std::map<GameObjectGlobalID, GameObject> Message::parseMessage() {
	//TODO: parse JSON data from the client's message and calculate updates to return accordingly
	return std::map<GameObjectGlobalID, GameObject>();
}