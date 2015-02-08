#include "Common.hpp"

#include "Game.hpp"
#include "ImageLoader.hpp"
int main(int argc, char **argv) {
	ImageLoader imgLoader;
	imgLoader.loadImage();

	Game game;
	game.init();
	game.run();
	return 0;
}