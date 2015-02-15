#ifndef COMMON_H
#define COMMON_H

#include "CommonMinimal.hpp"
#include <Windows.h>
#include "..\Dependencies\glew\glew.h"
#include "..\Dependencies\GLFW\glfw3.h"

//Include GLM
//GLM Fix now present in Minimal file.
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/vector_relational.hpp>

//Enable debug mode
#define DEBUG

#endif
