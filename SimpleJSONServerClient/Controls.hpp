#include <string>

class Control {
	private:
		//up is down -> accelerate
		bool U_HELD;
		//down is down -> brake;
		bool D_HELD;
		//right is down -> turn right
		bool R_HELD;
		//left is down -> turn left
		bool L_HELD;
	public:
		Control();
		~Control();
		Control(const Control& other);
		void regKeyPress(char key);
		void regKeyUnpress(char key);
		bool *getCurrentControls();
};