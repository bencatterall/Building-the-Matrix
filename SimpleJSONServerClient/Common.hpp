#ifndef COMMON_H
#define COMMON_H

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
#include <Windows.h>
#endif
#include "../Building-the-Matrix/Dependencies/glew/glew.h"
#include "../Building-the-Matrix/Dependencies/GLFW/glfw3.h"

// GLM Fixes
// We need this define for glm 0.9.6.1, it appears that 32-bit msvc++ compiling is broken
// when not passed by reference
//
// see below for the issue on git.
// https://github.com/g-truc/glm/issues/283
#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS

// Enable tests
//#define TEST
//#define TEST_GRAPHICS
//#define TEST_PHYSICS
//#define TEST_UPDATES

// Enable debug mode
#define DEBUG

#define  INTEL_X86

#include "CommonMinimal.hpp"

typedef GameObjectGlobalID GameObjectID;

#endif






