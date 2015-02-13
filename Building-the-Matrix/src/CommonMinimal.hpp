#ifndef COMMON_MINIMAL_H
#define COMMON_MINIMAL_H

//We need this define for glm 0.9.6.1, it appears that 32-bit msvc++ compiling is broken
//when not passed by reference
//
//see below for the issue on git.
//https://github.com/g-truc/glm/issues/283
#define GLM_FORCE_PURE

typedef int GameObjectID;
typedef int GameObjectGlobalID;

#endif