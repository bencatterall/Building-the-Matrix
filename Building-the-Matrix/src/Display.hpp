#ifndef DISPLAY_H
#define DISPLAY_H
#include "Common.hpp"
#include "RenderableComponent.hpp"
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


	static void glfwErrorCallback(int error, const char* desc) {
		cerr << "ERROR: In GLFW: " << error << " " << desc << std::endl;
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		//Close the health and safety message
		ovrHSWDisplayState hswDisplayState;
		ovrHmd_GetHSWDisplayState(hmd, &hswDisplayState);
		if (hswDisplayState.Displayed)
			ovrHmd_DismissHSWDisplay(hmd);
	}

	void cleanup();
	void init();
public:
	Display();
	~Display();
	void updateRenderTarget(int width, int height);
	void Output();
	void convertQuaternionToMatrix(const float* quat, float* mat);
	void run(void);

	//To be overridden

	///
	/// Camera has correct orientation and eye level position
	/// now, this function moves it around the scene as needed.
	///

	virtual void  renderScene(glm::mat4 modelViewMatrix, glm::mat4 projectionMatrix) = 0;
};

#endif