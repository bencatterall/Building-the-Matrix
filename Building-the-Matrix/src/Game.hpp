#ifndef GAME_H
#define GAME_H

#include "Common.hpp"
#include "Cube.hpp"
#include "Display.hpp"
class Game : public Display {
	int numCubes = 10; // 50 * 50 * 50;
	Cube* cubes;
public:
	void init();
	//To be overridden
	void renderScene(glm::mat4 modelViewMatrix, glm::mat4 projectionMatrix) override;
};
#endif