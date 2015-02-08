#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <string>
#include <vector>
#include "Common.hpp"
class ImageLoader {


public:
	std::vector<unsigned char> loadImage(std::string filename, unsigned int& width, unsigned int& height);
};

#endif