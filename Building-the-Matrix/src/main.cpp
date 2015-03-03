#include "Common.hpp"

#include "Game.hpp"
#include "ImageLoader.hpp"
#include <freeglut/glut.h>
#include <string>
#ifndef TEST

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	Game game;
    if (argc < 5) {        
        std::cerr << "Usage: " << argv[0] << " <remote IP> <remote port> <local IP> <local port>\n";
        return 1;
    }

    std::string remote_IP = argv[1];
    int remote_port = std::stoi(argv[2], nullptr, 10);
    std::string local_IP = argv[3];
    int local_port = std::stoi(argv[4], nullptr, 10);

	game.init();
	game.run(remote_IP, remote_port, local_IP, local_port);
	
	return 0;
}
#endif //TEST