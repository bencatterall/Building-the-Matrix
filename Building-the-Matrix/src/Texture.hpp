#ifndef TEXTURE_H
#define TEXTURE_H

#include "Common.hpp"
#include "../Dependencies/glew/glew.h"
#include "../Dependencies/GLFW/glfw3.h"
#include "ImageLoader.hpp"

#include <string>

class Texture {
	std::vector<unsigned char> pixelData;

	GLuint textureID = 0;
	int width = 0;
	int height = 0;
public:
	Texture(std::string filename);

	std::vector<unsigned char> getData() {
		return pixelData;
	}

	GLuint getID() { return textureID; }
};

#endif