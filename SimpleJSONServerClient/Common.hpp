#ifndef COMMON_H
#define COMMON_H

#ifndef PLATFORM

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

#endif // PLATFORM

/*
// Enable tests
//#define TEST
//#define TEST_PHYSICS
//#define TEST_UPDATES
*/

// Enable debug mode
#define DEBUG

#define INTEL_X86

#include "CommonMinimal.hpp"

#endif







