#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include "Serializer.hpp"

#include <vector>

///
///	MESSAGE FORMAT:
///   Message type: int ( 4 bytes)
///   Messsage length : int (4 bytes). Size of data, not this header
///	  Message data follows
///   
/// All are in Network byte order
///
///
///
class UpdateManager {




public:
	enum class MessageType {
		LOGIN=1, LOGOUT, STATE_CHANGED, OBJECT_DELETED
	};

	std::vector<char> receiveMessage(char* data, int datasize);

	std::vector<char> buildLoginMessage();
	std::vector<char> buildLogoutMessage();
	std::vector<char> buildStateChangeMessage();
	std::vector<char> buildObjectDeletedMessage();
	/*
	void handleLoginMessage(char* buffer, int len);
	void handleLogoutMessage(char* buffer, int len);
	void handleStateChangedMessage(char* buffer, int len);
	void handleObjectDeletedMessage(char* buffer, int len);
	void handleKeyPressed(char* buffer, int len);
	void handleKeyReleased(char* buffer, int len);
	void handleHeadOrientation(char* buffer, int len);
	*/
};
#endif