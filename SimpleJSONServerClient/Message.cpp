#include "Message.hpp" 
#include <iostream>

int Message::getMessageSize() {
	return (this->messageSize);
}

Message::Message(Address client, std::map<GameObjectGlobalID, GameObject> message) {
	 //TODO: turn map into a json string
	 char *s = "JSON UPDATE EXAMPLE\0";
	 (this->client) = client;
	 (this->message) = s;
	 (this->messageSize) = 20;
}

Message::Message(Address client, char *message, int length){
	(this->client) = client;
	(this->message) = message;
	(this->messageSize) = length;
}

Message::Message(Address client, std::string event) {
	std::cout << "event was " << event << "\n";
	//TODO: make an ACK message in JSON depending on what event happened e.g. a user logged out
	if (event.compare("A FELLOW PLAYER HAS LEFT THE GAME")==0){
		std::cout << "sending A FELLOW PLAYER HAS LEFT THE GAME\n";
		(this->client) = client;
		(this->message) = "A FELLOW PLAYER HAS LEFT THE GAME\0";
		(this->messageSize) = 38;
	}
}

Address Message::getClient() {
	return (this->client);
}

char* Message::getMessage() {
	return (this->message);
}

std::map<GameObjectGlobalID, GameObject> Message::parseMessage() {
	//TODO: parse JSON data from the client's message and calculate updates to return accordingly
	return std::map<GameObjectGlobalID, GameObject>();
}