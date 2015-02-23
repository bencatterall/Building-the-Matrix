#include "Common.hpp"

#include "Game.hpp"
#include "ImageLoader.hpp"
#include <freeglut/glut.h>
#ifndef TEST
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	Game game;
	game.init();
	game.run();
	
	return 0;
}
#endif //TEST