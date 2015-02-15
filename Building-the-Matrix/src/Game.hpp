#ifndef GAME_H
#define GAME_H

#include "Common.hpp"
#include "Chunk.hpp"
#include "Display.hpp"
class Game : public Display {
	int numChunks = 1;
	Chunk* chunks;
public:
	void init();
	//To be overridden
	void renderScene(glm::mat4 modelViewMatrix, glm::mat4 projectionMatrix) override;
};
#endif