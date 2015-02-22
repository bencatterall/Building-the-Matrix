#ifndef CHUNK_H
#define CHUNK_H
#include "Common.hpp"
#include "GameObject.hpp"
#include "TextureAtlas.hpp"

#include <memory>
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
	static const glm::vec3 cubeTopFace[6];
	static const glm::vec3 cubeBottomFace[6];
	static const glm::vec3 cubeLeftFace[6];
	static const glm::vec3 cubeRightFace[6];
	static const glm::vec3 cubeFrontFace[6];
	static const glm::vec3 cubeBackFace[6];

	static const GLfloat cubeFrontTextureCoords[6*2];
	static const GLfloat cubeBackTextureCoords[6 * 2];
	static const GLfloat cubeLeftTextureCoords[6 * 2];
	static const GLfloat cubeRightTextureCoords[6 * 2];
	static const GLfloat cubeTopTextureCoords[6 * 2];
	static const GLfloat cubeBottomTextureCoords[6 * 2];


	static const GLfloat cubeColours[4 * 36];

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

	void setupFaceData(int cubeX, int cubeY, int cubeZ, float xPos, float yPos, float zPos, int *numVertices,
		std::string tileName, std::shared_ptr<TextureAtlas> texture, const glm::vec3 (&vertexData)[6],
		const GLfloat (&texData)[6*2]);

public:
	Chunk();
	Chunk(double xPos, double yPos, double zPos);
};
#endif