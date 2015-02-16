#include "Sender.hpp"
#include <iostream>

Sender::Sender() {
	(this->cont) = true;
}

Sender::~Sender() {}

void Sender::setSocket(Socket s) {
	(this->serverSocket) = s;
}

void Sender::sendUpdateMessage(Address client, std::map<GameObjectGlobalID, GameObject> message) {
	Message m = Message(client, message);
	(this->toSend).pushToEnd(m);
}

void Sender::sendAck(Address client, std::string event) {
	Message m = Message(client, event);
	(this->toSend).pushToEnd(m);
}

//we only want one thread accessing send from the socket
void Sender::run() {
	while (this->cont) {
		try {
			Message m = (this->toSend).popFromFront();
			if (!((this->serverSocket).sendSingle(m.getClient(), m.getMessage(), sizeof(m.getMessage())))) {
				std::cerr << "Failed to send message";
			}
		}
		catch (int& i) {
			//don't try to send anything if there is nothing to send
		}
	}
}

void Sender::stop() {
	(this->cont) = false;
}