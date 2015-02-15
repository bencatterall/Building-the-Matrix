#ifndef CHUNK_H
#define CHUNK_H

#include "Common.hpp"
#include "GameObject.hpp"
#include <vector>

#define CHUNK_SIZE_X 10
#define CHUNK_SIZE_Y 10
#define CHUNK_SIZE_Z 10
///
/// Holds a set of cubes
///
class Chunk : public GameObject {

	///
	/// Cubes are building blocks of chunks, we just copy these where needed
	///
	static const glm::vec3 cubeData[36];
	static const GLfloat cubeColours[4 * 36];
	static const GLfloat cubeTextureCoords[36 * 2];


	///
	/// The world data
	/// used to do collision detection efficiently and optimisations/cleaning after
	/// noise generation
	///
	char chunkData[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];

	///
	///  Noise data, used to generate the world
	///
	double chunkNoiseData[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];

	void init();

public:
	Chunk();
	Chunk(double xPos, double yPos, double zPos);

	std::vector<GLfloat> chunkVertexData;
	std::vector<GLfloat> chunkColourData;
	std::vector<GLfloat> chunkTextureCoordsData;

};

#endif