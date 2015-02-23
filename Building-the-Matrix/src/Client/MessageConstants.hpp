#include "../Common.hpp"

class MessageConstants {
	private:
		static char* login;
		static char* logout;
		static char* pressed;
		static char* unpressed;
	public:
		MessageConstants();
		~MessageConstants();
		static const char *loginMessage();
		static const char *logoutMessage();
		static const char *keyPressMessage(char key);
		static const char *keyUnpressMessage(char key);
};