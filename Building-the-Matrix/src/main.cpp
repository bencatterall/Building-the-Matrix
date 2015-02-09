#include "Common.hpp"

#include "Game.hpp"
#include "ImageLoader.hpp"
#ifndef TEST
int main(int argc, char **argv) {

//game loop
	//networking get updates
	//update manager service updates
	//somoe form of prediction
	//handle user input
	//physics tick 
	//send updates to server
	//game render
//end loop

	Game game;
	game.init();
	game.run();
	return 0;
}
#endif