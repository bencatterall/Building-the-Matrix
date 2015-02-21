#include "GameObject.hpp"

class Player : public GameObject {
	private:
		Control control;
		float pitch;
		float roll;
		float yaw;
		int score;
	public:
		Player(GameObjectGlobalID id);
		~Player();
		Player(const Player& other);
		void setPRY(float pitch, float roll, float yaw);
		void keyPressed(char key);
		void keyUnpressed(char key);
		bool *getKeysPressed();
};