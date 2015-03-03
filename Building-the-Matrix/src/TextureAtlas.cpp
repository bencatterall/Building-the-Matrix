#include "TextureAtlas.hpp"

#include <fstream>
#include <iostream>
#include <utility>

TextureAtlas::TextureAtlas(std::string fileName, std::string nameMappings) : Texture(fileName) {
	//Get config file
	std::ifstream infile(nameMappings);
	if (!infile.is_open()){
		std::cerr << "ERROR: Couldn't load TextureAtlas config file" << std::endl;
		return;
	}

	//Read header
	infile >> tileWidth;
	infile >> tileHeight;
	infile >> numTilesX;
	infile >> numTilesY;
	infile >> format;
	infile >> numTiles;

	//Read body
	int index = 0;
	std::string name = "";
	while (infile >> index >> name) {
		//Add the (name, identifier) pair
		identififerNameMap.insert(std::pair<std::string, int>(name, index));
	}

	infile.close();
}


std::tuple<float, float, float, float> TextureAtlas::calculateTextureCoords(std::string name) {
	int id = 0;
	std::tuple<float, float, float, float> result = std::make_tuple(0.0f, 0.0f, 0.0f, 0.0f);

	try {
		id = identififerNameMap.at(name);
	}
	catch (...) {
		std::cerr << "ERROR: In TextureAtlas, name (" + name + ") not found in map!" << std::endl;
		return result;
	}

	if (id < 0 || id >= numTiles){
		std::cerr << "ERROR: Tile index out of range" << std::endl;
		return result;
	}

	//Calculate offsets

	//Size of a tile in relative floats
	float widthTile = 1.0f/ ((float)numTilesX);
	float heightTile = 1.0f / ((float)numTilesY);

	int xTileOffset = (id % (numTilesX));
	int yTileOffset = (id / (numTilesX));

	float xOffset = widthTile * (float)(xTileOffset);
	float yOffset = heightTile * (float)(yTileOffset);

	//Set new result
	result = std::make_tuple(xOffset, yOffset, widthTile, heightTile);

	return result;
}