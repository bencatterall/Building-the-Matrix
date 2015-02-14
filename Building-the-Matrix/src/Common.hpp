#ifndef COMMON_H
#define COMMON_H

#include <Windows.h>
#include "CommonMinimal.hpp"
#include "..\Dependencies\glew\glew.h"
#include "..\Dependencies\GLFW\glfw3.h"

//We need access to the native window handle so we need some native functions
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include "..\Dependencies\GLFW\glfw3native.h"

#define OVR_OS_WIN32
#include "OVR_CAPI.h"
#include "OVR_CAPI_GL.h"
#include "Kernel\OVR_Math.h"

//Include GLM

//GLM Fix now present in Minimal file.
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/vector_relational.hpp>

#include <iostream>

using namespace OVR;
using namespace std;


//Enable debug mode
#define DEBUG

//Enable tests
//#define TEST

//TODO: Remove this hack
extern ovrHmd hmd;

#endif
