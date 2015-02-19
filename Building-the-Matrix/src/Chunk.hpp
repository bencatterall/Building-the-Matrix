#ifndef CHUNK_H
#define CHUNK_H

#include "Common.hpp"
#include "GameObject.hpp"

#include <glm/glm.hpp>
#include <vector>

#define CHUNK_SIZE_X 256
#define CHUNK_SIZE_Y 150
#define CHUNK_SIZE_Z 256
///
/// Holds a set of cubes
///
class Chunk : public GameObject{

	

	///
	/// The world data
	/// used to do collision detection efficiently and optimisations/cleaning after
	/// noise generation
	///
	char chunkData[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	bool chunkVisibleData[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	///
	///  Noise data, used to generate the world
	///
	double chunkNoiseData[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];

	void init();

	///
	/// Cubes are building blocks of chunks, we just copy these where needed
	///
	static const glm::vec3 cubeData[36];
	static const GLfloat cubeColours[4 * 36];
	static const GLfloat cubeTextureCoords[36 * 2];

	///
	/// Hold the different faces
	///
	class ChunkData : public GameObject {

		std::vector<GLfloat> chunkVertexData;
		std::vector<GLfloat> chunkColourData;
		std::vector<GLfloat> chunkTextureCoordsData;
	};

	std::vector<GLfloat> chunkVertexData;
	std::vector<GLfloat> chunkColourData;
	std::vector<GLfloat> chunkTextureCoordsData;
	ChunkData chunkDatas[6];

public:
	Chunk();
	Chunk(double xPos, double yPos, double zPos);


};

#endif