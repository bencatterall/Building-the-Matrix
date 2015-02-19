#include <string>

class Control {
	private:
		//space is down -> accelerate
		bool S_HELD;
		//control is down -> brake;
		bool C_HELD;
		//right is down -> turn right
		bool R_HELD;
		//left is down -> turn left
		bool L_HELD;
	public:
		Control();
		~Control();
		void regKeyPress(char key);
		void regKeyUnpress(char key);
		bool *getCurrentControls();
};