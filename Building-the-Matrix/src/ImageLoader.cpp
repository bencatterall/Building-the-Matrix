#include "ImageLoader.hpp"
#include "../Dependencies/LodePNG/lodepng.hpp"

#include <string>
#include <vector>

std::vector<unsigned char> ImageLoader::loadImage(std::string filename, unsigned int& width, unsigned int& height) {
	//raw pixels in RGBA format
	//4 bytes per pixel
	std::vector<unsigned char> image;

	//decode
	unsigned error = lodepng::decode(image, width, height, filename.c_str());

	if (error) {
		std::cerr << "ERROR: In loadImage: " << lodepng_error_text(error) << std::endl;
		return image;
	}

	return image;
}
