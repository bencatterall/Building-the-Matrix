#include "Common.hpp"
#include "GameObject.hpp"
#include "UpdateManager.hpp"
#include "Serializer.hpp"


#include <assert.h>
#include <iostream>
#include <type_traits> //for std::underlying_type


std::vector<char> UpdateManager::receiveMessage(char* data, int datasize) {
	int dataPtr = 0;
	//TODO: Check bounds, possible vulnerability if not
	MessageType type = static_cast<MessageType>(Serializer::unpackInt(data, dataPtr));
	unsigned int length = (unsigned int)Serializer::unpackInt(data + dataPtr, dataPtr);

	std::vector<char> result;

	//Message data follows
	if (length > (unsigned int)datasize - 8) {
		std::cerr << "ERROR: In Receivemessage, length too long" << std::endl;
		assert(false);
		//Don't handle the packet, drop it
		return result;
	}

	//handle the message
	switch (type) {
	case MessageType::LOGIN:
	//	handleLoginMessage(data + dataPtr, length);
		break;

	case MessageType::LOGOUT:
	//	handleLogoutMessage(data + dataPtr, length);
		break;

	case MessageType::STATE_CHANGED:
	//	handleStateChangedMessage(data + dataPtr, length);
		break;

	case MessageType::OBJECT_DELETED:
	//	handleObjectDeletedMessage(data + dataPtr, length);
		break;

	default:
		std::cerr << "ERROR: In Receivemessage, Unrecognised enum type" << std::endl;
		assert(false);
		//Don't handle the packet, drop it
		return result;
	}

	return result;
}

std::vector<char> UpdateManager::buildLoginMessage() {
	MessageType type = MessageType::LOGIN;
	int value = static_cast<std::underlying_type<MessageType>::type>(type);

	std::vector<char> data;
	return data;
}

std::vector<char> UpdateManager::buildLogoutMessage() {
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
/*
void handleLoginMessage(char* buffer, int len) {
	std::cout << "Server received login request \n";

	GameObjectGlobalID id;
	bool copy = false;

	for (Address s : clients) {
		if ((s.getAddress() == recFrom.getAddress()) && (s.getPort() == recFrom.getPort())) {
			//already received request but ACK not received by sender so just try again
			std::cout << "resending login ACK \n";
			copy = true;
			for (std::pair<Address, GameObjectGlobalID> e : playerIDs) {
				if (e.first.getAddress() == recFrom.getAddress() && e.first.getPort() == recFrom.getAddress()) {
					id = e.second;
				}
			}
			break;
		}
	}
	if (!copy) {
		std::cout << "Added client to current list connected with address = " << recFrom.getHBOAddress() << " at port = " << recFrom.getHBOPort() << "\n";
		clients.push_back(recFrom);
		//create their car for the game, generate a global ID too
		id = updateManager.getNextObjectID();
		playerIDs.push_back(std::pair<Address, GameObjectGlobalID>(recFrom, id));
		updateManager.queueUpdate(GameObject(id, true));
	}
	char data[] = "LOGIN ACCEPTED     ";
	char *idparts = (char*)&id;
	data[15] = idparts[0];
	data[16] = idparts[1];
	data[17] = idparts[2];
	data[18] = idparts[3];
	sender.sendMessage(recFrom, data, 20);
}

void handleLogoutMessage(char* buffer, int len) {
	std::cout << "Server received logout request \n";
	std::vector<Address>::iterator it;
	std::vector<std::pair<Address, GameObjectGlobalID>>::iterator it2;
	for (it = clients.begin(); it < clients.end(); it++) {
		if ((it->getAddress() == recFrom.getAddress()) && (it->getPort() == recFrom.getPort())) {
			clients.erase(it);
			std::cout << "removed from current connected clients \n";
			break;
		}
		else {
			const char data[] = "A FELLOW PLAYER HAS LEFT THE GAME";
			sender.sendAck((*it), data);
		}
	}
	for (it2 = playerIDs.begin(); it2 < playerIDs.end(); it2++) {
		if (((it2->first).getAddress() == recFrom.getAddress()) && ((it2->first).getPort() == recFrom.getPort())) {
			updateManager.remove(it2->second);
			playerIDs.erase(it2);
			std::cout << "removed from current IDs \n";
			break;
		}
	}
}

void handleStateChangedMessage(char* buffer, int len){

}

void handleObjectDeletedMessage(char* buffer, int len) {

}

void handleKeyPressed(char* buffer, int len) {
	char key = buffer[8];
	std::cout << "User pressed " << key << "\n";
	for (std::pair<Address, GameObjectGlobalID> e : playerIDs) {
		if (e.first.getAddress() == recFrom.getAddress() && e.first.getPort() == recFrom.getAddress()) {
			GameObject o = updateManager.getGameObject(e.second);
			o.keyPressed(key);
			updateManager.queueUpdate(o);
			break;
		}
	}
}

void handleKeyReleased(char* buffer, int len) {
	char key = buffer[10];
	std::cout << "User unpressed " << key << "\n";
	for (std::pair<Address, GameObjectGlobalID> e : playerIDs) {
		if (e.first.getAddress() == recFrom.getAddress() && e.first.getPort() == recFrom.getAddress()) {
			GameObject o = updateManager.getGameObject(e.second);
			o.keyUnpressed(key);
			updateManager.queueUpdate(o);
			break;
		}
	}
}

void handleHeadOrientation(char* buffer, int len) {
	std::cout << "recieved pitch, roll and yaw from " << recFrom.getHBOAddress() << " " << recFrom.getHBOPort() << "\n";
	float *pry = new float[3];
	pry[0] = (float)((buffer[3] << 24) | (buffer[4] << 16) | (buffer[5] << 8) | buffer[6]);
	pry[1] = (float)((buffer[7] << 24) | (buffer[8] << 16) | (buffer[9] << 8) | buffer[10]);
	pry[2] = (float)((buffer[11] << 24) | (buffer[12] << 16) | (buffer[13] << 8) | buffer[14]);
	std::cout << "p=" << pry[0] << " r=" << pry[1] << " y=" << pry[2] << "\n";
	for (std::pair<Address, GameObjectGlobalID> e : playerIDs) {
		if (e.first.getAddress() == recFrom.getAddress() && e.first.getPort() == recFrom.getAddress()) {
			GameObject o = updateManager.getGameObject(e.second);
			o.setPRY(pry[0], pry[1], pry[2]);
			updateManager.queueUpdate(o);
			break;
		}
	}
}*/