#include "Chunk.hpp"
#include "../../Building-the-Matrix/src/SimplexNoise.hpp"

#include <assert.h>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

Chunk::Chunk()
	: Chunk(0.0, 0.0, 0.0)
{

}

Chunk::Chunk(double xPos, double yPos, double zPos) :
	xPos(xPos), yPos(yPos), zPos(zPos)
{
	init();
}

bool Chunk::cubeAt(float xPos, float yPos, float zPos) {
	double cubeSize = getCubeSize();
	int xLength = getChunkSizeX(), yLength = getChunkSizeY(), zLength = getChunkSizeZ();
	int xLoc = (int)((xPos - this->xPos) / cubeSize) + xLength / 2;
	int yLoc = (int)((yPos - this->yPos) / cubeSize) + yLength / 2;
	int zLoc = (int)((zPos - this->zPos) / cubeSize) + zLength / 2;

	assert(xLoc >= 0 && yLoc >= 0 && zLoc >= 0);
	assert(xLoc < xLength && yLoc < yLength && zLoc < zLength);

	return (chunkData[xLoc][yLoc][zLoc] > 0);
}

void Chunk::init() {
	//Map of cube types (GRASS, WATER etc) to a string identifer
	std::map<int, std::string> cubeTypeToId;
	cubeTypeToId.insert(std::make_pair<int, std::string>(1, std::string("grass")));
	cubeTypeToId.insert(std::make_pair<int, std::string>(2, std::string("sand")));
	cubeTypeToId.insert(std::make_pair<int, std::string>(3, std::string("stone")));
	cubeTypeToId.insert(std::make_pair<int, std::string>(4, std::string("water")));

	//Dimensions
	int xLength = getChunkSizeX(), yLength = getChunkSizeY(), zLength = getChunkSizeZ();
	double xScale = 80.0;
	double zScale = 80.0;
	double yScale = 0.3;
	SimplexNoise noiseGenerator;

	//Generate the world 
	for (int x = -xLength / 2; x < xLength / 2; ++x) {
		for (int z = -zLength / 2; z < zLength / 2; ++z) {
			//Generate noise 
			//[-1.0,1.0]
			double noise = (noiseGenerator.noise2D(x / xScale, z / zScale));

			//fill in y and below this block
			int yHeight = (int)(noise*(yLength / 2) * yScale);
			for (int y = -yLength / 2; y < yHeight; ++y) {

				if (y < 4) {
					//Put sand here
					chunkData[x + (xLength / 2)][y + yLength / 2][z + (zLength / 2)] = 2;
				}
				else {
					chunkData[x + (xLength / 2)][y + yLength / 2][z + (zLength / 2)] = 1;
				}
			}

			//clear above
			for (int y = yHeight; y < yLength; ++y) {
				chunkData[x + (xLength / 2)][y + yLength / 2][z + (zLength / 2)] = 0;
			}

			//Set data
			chunkNoiseData[x + (xLength / 2)][yHeight + yLength / 2][z + (zLength / 2)] = noise;
		}
	}

	//Put water on center
	for (int x = -xLength / 2; x < xLength / 2; ++x) {
		for (int z = -zLength / 2; z < zLength / 2; ++z) {
			if (chunkData[x + (xLength / 2)][yLength / 2][z + (zLength / 2)] == 0) {
				chunkData[x + (xLength / 2)][yLength / 2][z + (zLength / 2)] = 4;
			}
		}
	}

	//Optimise and clean up the world
	for (int x = -xLength / 2; x < xLength / 2; ++x) {
		for (int z = -zLength / 2; z < zLength / 2; ++z) {
			for (int y = -yLength / 2; y < yLength / 2; ++y) {
				//perform clustering
				int baseX = x + (xLength / 2);
				int baseY = y + (yLength / 2);
				int baseZ = z + (zLength / 2);

				//If our cube has no entry
				if (chunkData[baseX][baseY][baseZ] == 0) {
					chunkVisibleData[baseX][baseY][baseZ] = false;
					continue;
				}

				chunkVisibleData[baseX][baseY][baseZ] = false;
				//hide cubes if there is a cube:
				//above and below
				//in front and behind
				//to the left and the right
				if (baseX - 1 > 0) {
					if (chunkData[baseX - 1][baseY][baseZ] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				if (baseX + 1 < xLength) {
					if (chunkData[baseX + 1][baseY][baseZ] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				if (baseY - 1 > 0) {
					if (chunkData[baseX][baseY - 1][baseZ] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				if (baseY + 1 < yLength) {
					if (chunkData[baseX][baseY + 1][baseZ] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				if (baseZ - 1 > 0)  {
					if (chunkData[baseX][baseY][baseZ - 1] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				if (baseZ + 1 < zLength) {
					if (chunkData[baseX][baseY][baseZ + 1] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}

			}
		}
	}
}