#include "GameObject.hpp"

class Player : public GameObject {
	private:
		KeyboardControl control;
		float pitch;
		float roll;
		float yaw;
		int score;
	public:
		Player(GameObjectGlobalID id);
		Player(unsigned char *serial, int &size);
		~Player();
		Player(const Player& other);
		void setPRY(float pitch, float roll, float yaw);
		void keyPressed(char key);
		void keyUnpressed(char key);
		bool *getKeysPressed();
		int serialize(unsigned char* buffer);
		int deserialize(unsigned char* buffer);
};