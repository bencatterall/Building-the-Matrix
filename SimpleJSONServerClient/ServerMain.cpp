/**
	ServerMain.cpp
	Purpose: Controls a server instance bounded to arg[1] address and arg[2] port number

*/

#include "Address.hpp"
#include "Socket.hpp"
#include "SafeMap.hpp"
#include "SafeQueue.hpp"
#include "CommonMinimal.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

SafeMap<GameObjectGlobalID, GameObject> objects;
SafeQueue<Update> pendingUpdates;
bool cont;

void reader() {
	while (cont) {
		try {
			pendingUpdates.popFromFront();
		}
		catch (...) {
			//ignore for now
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

}

void writerAdd() {
	while (cont) {
		//int r = std::rand() % 10;
		Update u = Update();
		pendingUpdates.pushToEnd(u);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void writerDelete() {
	while (cont) {
		int r = std::rand() % 10;
		objects.deleteEntry(r);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}
/*
int main(int argc, char **argv) {
	cont = true;
	std::thread snap(reader);
	std::thread writer1(writerAdd);
	//std::thread writer2(writerDelete);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	cont = false;
	snap.join();
	writer1.join();
	//writer2.join();
	if (std::cin.get() == '\n') {}
} */

/*
//runs continuously to deal with packets received
void receive() {
	while (cont) {
		Address sender;
		unsigned char buffer[256];
		int bytes_read = mySocket.receive(sender, (char *)buffer, sizeof(buffer));
		
		if (bytes_read >= 0) {
			//check what type of message was received
			std::string message((char *)buffer);
			
			if (message.compare("LOGIN") == 0) {
				bool copy = false;
				std::cout << "Server received login request \n";
				for (Address s : clients) {
					if ((s.getAddress() == sender.getAddress()) && (s.getPort() == sender.getPort())) {
						//already received request but ACK not received by sender so just try again
						std::cout << "resending login ACK \n";
						copy = true;
						break;
					}
				}
				if (!copy) {
					std::cout << "Added client to current list connected with address = " << sender.getHBOAddress() << " at port = " << sender.getHBOPort() << "\n";
					clients.push_back(sender);
				}
				const char data[] = "LOGIN ACCEPTED";
				if (!(mySocket.sendSingle(sender, data, sizeof(data)))) {
					std::cerr << "Failed to send message\n";
					std::exit(EXIT_FAILURE);
				}
			}

			else if (message.compare("LOGOUT") == 0) {
				std::cout << "Server received logout request \n";
				std::vector<Address>::iterator it;
				for (it = clients.begin(); it < clients.end(); it++) {
					if ((it->getAddress() == sender.getAddress()) && (it->getPort() == sender.getPort())) {
						clients.erase(it);
						std::cout << "removed from current connected clients \n";
						break;
					}
				}
			}

			else {
				std::cout << "Unknown message received\n";
			}
		}
	}
}

//used to send updates at particular interval
void broadcast() {
	while (cont) {
		for (Address client : clients) {
			//send snapshot back - temporarily just this message
			const char data[] = "{NumObjects:1,{ObjectID:abc123,field1:a,field2:1}}";
			if (!(mySocket.sendSingle(client, data, sizeof(data)))) {
				std::cerr << "Failed to send message";
				std::exit(EXIT_FAILURE);
			}
		}
		//should be milliseconds - for testing purposes only
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

//keeps this server running
int main(int argc, char **argv) {
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
		IPValues[acc] = atoi((IP.substr(0,pos)).c_str());
		IP = IP.substr(pos+1, (IP.length() - (pos+1)));
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
	if (!mySocket.openSocket(serverAddr->getAddress(),serverAddr->getPort()))
	{
		std::cerr << "Failed to create server socket\n";
		std::exit(EXIT_FAILURE);
	}

	//start receiving packets
	cont = true;
	std::thread rec(receive);


	//TODO: set up initial objects here


	//start loop of sending packets at each interval
	std::thread send(broadcast);

	//for now, quit when enter is pressed
	if (std::cin.get() == '\n') {
		cont = false;
	}

	rec.join();
	send.join();

	std::cout << "quitting server with connected clients:\n";
	for (Address a : clients) {
		std::cout << a.getHBOAddress() << "\t" << a.getHBOPort();
	}
	mySocket.~Socket();
}

*/