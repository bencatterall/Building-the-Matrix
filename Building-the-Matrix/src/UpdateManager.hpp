#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include "Serializer.hpp"

#include <vector>
class UpdateManager {



public:
	enum MessageType {

	};



	std::vector<char> buildLoginMessage();
	std::vector<char> buildLogoutMessage();
	std::vector<char> buildStateChangeMessage();
	std::vector<char> buildMessage();
};
#endif