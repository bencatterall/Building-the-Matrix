#include "Common.hpp"

#include "Game.hpp"
#include "ImageLoader.hpp"
#ifndef TEST
int main(int argc, char **argv) {

	Game game;
	game.init();
	game.run();
	return 0;
}
#endif