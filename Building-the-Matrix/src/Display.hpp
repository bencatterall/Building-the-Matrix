#ifndef DISPLAY_H
#define DISPLAY_H
#include <iostream>

#include "Common.hpp"
#include "RenderableComponent.hpp"


//We need access to the native window handle so we need some native functions
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include "..\Dependencies\GLFW\glfw3native.h"

#define OVR_OS_WIN32
#include "OVR_CAPI.h"
#include "OVR_CAPI_GL.h"
#include "Kernel\OVR_Math.h"

#include <glm/glm.hpp>

using namespace OVR;
//TODO: Remove this hack
extern ovrHmd hmd;

class Display {
	ovrFrameTiming frameTiming;
	ovrEyeRenderDesc eyeRenderDesc[2];
	ovrGLTexture frameBufferOvrTexture[2];
	GLuint frameBufferObject = 0;
	GLuint frameBufferTexture = 0;
	GLuint frameBufferDepth = 0;
	int frameBufferWidth = 0;
	int frameBufferHeight = 0;
	GLFWwindow* window = nullptr;
	glm::vec3 headOrientation;

	static void glfwErrorCallback(int error, const char* desc) {
		std::cerr << "ERROR: In GLFW: " << error << " " << desc << std::endl;
	}

	void cleanup();
	void init();
	///
	/// Vector of yaw, pitch, roll in radians
	///
	glm::vec3 getHeadOrientation() { return headOrientation;  }
	void findHeadOrientation();
public:
	Display();
	virtual ~Display();
	void updateRenderTarget(int width, int height);
	void Output();
	void convertQuaternionToMatrix(const float* quat, float* mat);
	void run(void);

	///
	/// Call code to begin the render sequence
	///
	void render(void);
	//To be overridden

	///
	/// Camera has correct orientation and eye level position
	/// now, this function moves it around the scene as needed.
	///

	virtual void  renderScene(glm::mat4 modelViewMatrix, glm::mat4 projectionMatrix) = 0;
};

#endif