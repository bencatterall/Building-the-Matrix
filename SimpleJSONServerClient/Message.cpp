#include "Message.hpp" 

Message::Message(Address client, std::map<GameObjectGlobalID, GameObject> message) {
	 //TODO: turn map into a json string
	 char *s = "JSON UPDATE EXAMPLE";
	 (this->client) = client;
	 (this->message) = s;
}

Message::Message(Address client, std::string event) {
	//TODO: make an ACK message in JSON depending on what event happened e.g. a user logged out
	std::string s = "JSON EVENT EXAMPLE";
	(this->client) = client;
	(this->message) = "example";
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