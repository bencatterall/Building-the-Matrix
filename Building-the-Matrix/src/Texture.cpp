#include "Texture.hpp"

Texture::Texture(std::string filename) {
	ImageLoader imgLoader;
	unsigned int width, height;
	std::vector<unsigned char> image = imgLoader.loadImage(filename, width, height);

	//handle no image
	if (image.size() == 0) {
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		this->width = 0;
		this->height = 0;
		pixelDataSize = 0;
		pixelData = new unsigned char[pixelDataSize];

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		return;
	}
	//Copy data across
	//TODO: Stop all this copying
	this->width = width;
	this->height = height;
	pixelDataSize = image.size();
	pixelData = new unsigned char[image.size()];
	int i = 0;
	for (unsigned char pixelComponent : image) {
		pixelData[i++] = pixelComponent;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}