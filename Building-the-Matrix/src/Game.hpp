#ifndef GAME_H
#define GAME_H

#include <memory>

#include "Common.hpp"
#include "Chunk.hpp"
#include "Display.hpp"
#include "Player.hpp"

class Game : public Display {
protected:
	int numChunks = 1;
	Chunk* chunks;
	//TODO: Fix this hack. THis is because networking needs to be moved out of Display but,
	//time constraints mean we can't
public:
	std::shared_ptr<Player> player;
public:
	virtual ~Game() {};
	std::shared_ptr<Player> getPlayer() { return player; }
	void setPlayer(std::shared_ptr<Player> newPlayer) { player = newPlayer; }
	void init();
	//To be overridden
	void renderScene(glm::mat4 modelViewMatrix, glm::mat4 projectionMatrix) override;
};
#endif