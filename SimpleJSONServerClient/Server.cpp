#include "Server.hpp"
#include "UpdateManager.hpp"
#include "Address.hpp"
#include "Sender.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

UpdateManager updateManager;
Socket mySocket;
std::vector <Address> clients;
Sender sender;
bool cont;

void broadcast() {
	while (cont) {
		for (Address client : clients) {
			//send snapshot back
			std::map<GameObjectGlobalID, GameObject> toSend = updateManager.flushUpdates();
			sender.sendUpdateMessage(client, toSend);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void quit() {
	if (std::cin.get() == '\n') {
		cont = false;
	}
}

int main(int argc, char **argv) {
	//setup server to run on port
	//expect IP address to be the localhost of the machine: "127.0.0.1"
	if (argc != 3) {
		std::cerr << "Usage: create server by providing <IP address> <Port>\n";
		std::exit(EXIT_FAILURE);
	}

	std::string IP = argv[1];
	std::string port = argv[2];
	unsigned short portNo = atoi(port.c_str());
	int acc = 0;
	int IPValues[4];
	size_t pos = 0;
	while ((pos = IP.find('.')) != std::string::npos) {
		IPValues[acc] = atoi((IP.substr(0, pos)).c_str());
		IP = IP.substr(pos + 1, (IP.length() - (pos + 1)));
		acc++;
	}
	IPValues[3] = atoi(IP.c_str());
	acc++;
	if (acc != 4) {
		std::cerr << "Deformed IP address given\n";
		std::exit(EXIT_FAILURE);
	}

	//make address for server to run on from commandline arguments (alternatively we could make a
	//config file?)
	Address *serverAddr = new Address(IPValues[0], IPValues[1], IPValues[2], IPValues[3], portNo);

	// create socket server will be on
	if (!mySocket.openSocket(serverAddr->getAddress(), serverAddr->getPort()))
	{
		std::cerr << "Failed to create server socket\n";
		std::exit(EXIT_FAILURE);
	}

	std::vector < GameObject > initialObjects;
	//TODO: push initial objects

	updateManager = UpdateManager(initialObjects);
	sender = Sender(mySocket);
	//start update manager run method in new thread
	std::thread makeUpdates(&UpdateManager::run, updateManager);

	//start send loop in new thread
	std::thread sendMessages(&Sender::run, sender);

	//start send update loop in new thread
	std::thread update(broadcast);

	//start listener fo quitting
	std::thread quit(quit);

	//start receive then update loop in this thread
	while (cont) {
		//try to receive updates
		Address recFrom;
		unsigned char buffer[256];
		int bytes_read = mySocket.receive(recFrom, (char *)buffer, sizeof(buffer));

		if (bytes_read >= 0) {
			//check what type of message was received
			std::string message((char *)buffer);

			if (message.compare("LOGIN") == 0) {
				bool copy = false;
				std::cout << "Server received login request \n";
				for (Address s : clients) {
					if ((s.getAddress() == recFrom.getAddress()) && (s.getPort() == recFrom.getPort())) {
						//already received request but ACK not received by sender so just try again
						std::cout << "resending login ACK \n";
						copy = true;
						break;
					}
				}
				if (!copy) {
					std::cout << "Added client to current list connected with address = " << recFrom.getHBOAddress() << " at port = " << recFrom.getHBOPort() << "\n";
					clients.push_back(recFrom);
				}
				const char data[] = "LOGIN ACCEPTED";
				sender.sendAck(recFrom, data);
			}

			else if (message.compare("LOGOUT") == 0) {
				std::cout << "Server received logout request \n";
				std::vector<Address>::iterator it;
				for (it = clients.begin(); it < clients.end(); it++) {
					if ((it->getAddress() == recFrom.getAddress()) && (it->getPort() == recFrom.getPort())) {
						clients.erase(it);
						std::cout << "removed from current connected clients \n";
						break;
					}
					else {
						sender.sendAck((*it),"A fellow player has left the game");
					}
				}
			}

			else {
				std::cout << "Unknown message received\n";
			}
		}
		//call updateManager accordingly
		//run physics
	}

	std::cout << "quitting the server";
	updateManager.stop();
	sender.stop();
	makeUpdates.join();
	sendMessages.join();
	update.join();
	quit.join();
	std::cout << "server killed";
}
