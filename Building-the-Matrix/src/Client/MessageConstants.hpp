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
		static const unsigned char *keyPressMessage(int);
		static const unsigned char *keyUnpressMessage(int);
};