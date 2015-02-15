#ifndef GAME_H
#define GAME_H

#include "Common.hpp"
#include "Cube.hpp"
#include "Display.hpp"
class Game : public Display {
	int numCubes = 6*6*6;
	Cube cubes[1000];
public:
	void init();
	//To be overridden
	void renderScene(glm::mat4 modelViewMatrix, glm::mat4 projectionMatrix) override;
};
#endif