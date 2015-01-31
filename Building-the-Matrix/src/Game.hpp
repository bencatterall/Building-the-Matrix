#ifndef GAME_H
#define GAME_H

#include "Common.hpp"
#include "Display.hpp"
class Game : public Display {
	int numCubes = 1;
	RenderableComponent* cubes[1];
public:
	void init();
	//To be overridden
	void renderScene(glm::mat4 modelViewMatrix, glm::mat4 projectionMatrix) override;
};
#endif