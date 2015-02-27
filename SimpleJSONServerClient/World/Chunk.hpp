#ifndef CHUNK_H
#define CHUNK_H

#include "../../Building-the-Matrix/Dependencies/glm/glm.hpp"
#include <memory>
#include <vector>

#define CHUNK_SIZE_X 256
#define CHUNK_SIZE_Y 150
#define CHUNK_SIZE_Z 256
#define CUBE_HALF_SIZE 10.0f
///
/// Holds a set of cubes
///
class Chunk{
	double xPos = 0.0;
	double yPos = 0.0;
	double zPos = 0.0;
	///
	/// The world data used to do collision detection efficiently
	/// and optimisations/cleaning after noise generation
	///
	char chunkData[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	bool chunkVisibleData[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	///
	/// Noise data, used to generate the world
	///
	double chunkNoiseData[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	void init();
	///
	/// Cubes are building blocks of chunks, we just copy these where needed
	///

public:
	int getChunkSizeX() { return CHUNK_SIZE_X; }
	int getChunkSizeY() { return CHUNK_SIZE_Y; }
	int getChunkSizeZ() { return CHUNK_SIZE_Z; }
	//Cubes range between 1 and -1 and CUBE_HALF_SIZE is the size of 1 to 0 scaled up by cubesize
	float getCubeSize() { return CUBE_HALF_SIZE * 2; }
	bool cubeAt(glm::vec3 worldLocation);
	glm::vec3 getCubeCenter(glm::vec3 worldLocation);
	Chunk();
	Chunk(double xPos, double yPos, double zPos);
};
#endif