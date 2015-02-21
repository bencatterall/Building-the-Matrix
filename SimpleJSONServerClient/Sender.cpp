#include "Sender.hpp"
#include <iostream>

Sender::Sender() {
	(this->cont) = true;
}

Sender::~Sender() {}

void Sender::setSocket(Socket *s) {
	(this->serverSocket) = *s;
}

void Sender::sendUpdateMessage(Address client, std::map<GameObjectGlobalID, GameObject> message) {
	//Message m = Message(client, message);
	Message m = Message(client,"UPDATE MESSAGE",14);
	(this->toSend).pushToEnd(m);
}

void Sender::sendAck(Address client, std::string event) {
	Message m = Message(client, event);
	(this->toSend).pushToEnd(m);
}

void Sender::sendMessage(Address client, char *message, int length){
	Message m = Message(client, message, length);
	(this->toSend).pushToEnd(m);
}

//we only want one thread accessing send from the socket
void Sender::run() {
	while (this->cont) {
		if (!(this->toSend).isEmpty()) {
			Message m = (this->toSend).popFromFront();
			if (!((this->serverSocket).sendSingle(m.getClient(), m.getMessage(), m.getMessageSize()))) {
				std::cout << "Failed to send message\n";
			}
		}
	}
}

void Sender::stop() {
	std::cout << "Called stop on sender";
	(this->cont) = false;
}