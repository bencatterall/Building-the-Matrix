#ifndef CUBE_H
#define CUBE_H
#include "Common.hpp"
#include "GameObject.hpp"
#include <vector>
#include <glm/glm.hpp>
///
/// TODO IMPROVE
///This is just (REALLY!) inefficient test code
class Cube : public GameObject {
	///
	/// Test data
	///
	/**
	* The vertices are needed in a counter-clockwise facing orientation
	* 6 --- 7
	* /| /|
	* 2 --- 3 |
	* | 4 --|-5
	* |/ |/
	* 0 --- 1
	*
	*Faces:
	*
	*
	* 2---3
	* |   |
	* |   |
	* 0---1
	* is then
	*
	* 2---3
	* | /
	* | /
	* 0
	* 0-3-2
	* 3
	* / |
	* / |
	* 0 --1
	* 0-1-3
	*/
	static const GLfloat cubeData[3 * 36];
	static const GLfloat cubeColours[4 * 36];
	static const GLfloat cubeTextureCoords[36 * 2];
public:
	Cube(glm::vec3 pos, float sf);
	//TODO:
	//Fix this hack
	std::vector<GLfloat> cubeVertexData;
	std::vector<GLfloat> cubeColourData;
	std::vector<GLfloat> cubeTextureCoordsData;
};
#endif