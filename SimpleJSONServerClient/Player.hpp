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
		float getPitch();
		float getRoll();
		float getYaw();
		void keyPressed(int key);
		void keyUnpressed(int key);
		// bool *getKeysPressed();
		int serialize(unsigned char* buffer);
		int deserialize(unsigned char* buffer);
		void setControl(KeyboardControl control);
		KeyboardControl getControl();
		int getScore() const;
		void modifyScore(int delta);
		bool getUp();
		bool getDown();
		bool getLeft();
		bool getRight();
		bool getKey(unsigned int);
};