#include "ImageLoader.hpp"
#include "../Dependencies/LodePNG/lodepng.hpp"

#include <string>
#include <vector>

bool ImageLoader::loadImage() {
	//raw pixels in RGBA format
	//4 bytes per pixel
	std::vector<unsigned char> image;

	std::string filename = "resources/textures/grass.png";
	unsigned int width, height;
	//decode
	unsigned error = lodepng::decode(image, width, height, filename.c_str());

	if (error) {
		std::cerr << "ERROR: In loadImage: " << lodepng_error_text(error) << std::endl;
		return false;
	}

	std::cout << "Loaded" << std::endl;
	return true;
}
