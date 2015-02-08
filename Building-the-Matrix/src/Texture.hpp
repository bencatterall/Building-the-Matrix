#ifndef TEXTURE_H
#define TEXTURE_H

#include "Common.hpp"
#include "ImageLoader.hpp"

#include <string>

class Texture {
	unsigned char* pixelData = nullptr;
	int pixelDataSize = 0;
	GLuint textureID = 0;
	int width = 0;
	int height = 0;
public:
	Texture(std::string filename);

	unsigned char* getData() {
		return pixelData;
	}

	GLuint getID() { return textureID; }
};

#endif