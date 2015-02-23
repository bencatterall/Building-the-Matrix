#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "Texture.hpp"

#include <map>
#include <string>
#include <utility>


class TextureAtlas : public Texture {
	int tileWidth = 0;
	int tileHeight = 0;
	int numTilesX = 0;
	int numTilesY = 0;
	int numTiles = 0;
	std::string format = "";
	std::map<std::string, int> identififerNameMap;

public:
	TextureAtlas(std::string fileName);

	///
	/// Calculate texture coordinate offsets for a named tile
	/// (x, y, width, height) 
	///
	std::tuple<float, float, float, float> calculateTextureCoords(std::string name);

	int getTileWidth() { return tileWidth; }
	int getTileHeight() { return tileHeight; }
	int getNumTilesX() { return numTilesX;  }
	int getNumTilesY() { return numTilesY; }
	int getNumTiles() { return numTiles;  }
};


#endif