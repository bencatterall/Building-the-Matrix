#include "Client/Client.hpp"
#include "Display.hpp"
#include "ObjectManager.hpp"
#include "JSON/UpdateManager.hpp"
#include "../src/Common.hpp"
#include "Serializer.hpp"
#include "Player.hpp"
#include "Game.hpp"

#include <conio.h>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <freeglut/glut.h>
#include <freeglut/freeglut_ext.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <freeglut/glut.h>
ovrHmd hmd;

using namespace std;
Client client;
//asynchronous user input so not required in game loop
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);


	//Close the health and safety message
	ovrHSWDisplayState hswDisplayState;
	ovrHmd_GetHSWDisplayState(hmd, &hswDisplayState);
	if (hswDisplayState.Displayed)
		ovrHmd_DismissHSWDisplay(hmd);

	//handle user input
	//TODO: local movement needed, rather than just getting server to move us
	if (key != GLFW_KEY_UNKNOWN && key <= GLFW_KEY_LAST) {
		if (action == GLFW_PRESS) {
			client.sendKeyPress(key);
		} else if (action == GLFW_RELEASE) {
			client.sendKeyUnpress(key);
		}
	}
}

Display::Display() {
	init();
}
void Display::init() {
	//Initialise the OVR and create a HMD
	ovr_Initialize();
	hmd = ovrHmd_Create(0);

	if (!hmd) {
		cout << "Failed to open Oculus HMD, falling back onto virtual debug." << std::endl;
		hmd = ovrHmd_CreateDebug(ovrHmd_DK2);

		//Called when creating normally but not on debug
		ovrHmd_ResetFrameTiming(hmd, 0);
	}
	//If we failed to create a debug version
	if (!hmd) { 
		cerr << "Failed to init debug Oculus HMD." << std::endl; 
		//TODO: switch to desktop mode
		exit(EXIT_FAILURE);
	}

	//Setup tracking 
	ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_Position | ovrTrackingCap_MagYawCorrection, 0);

	//Get HMD properties
	bool extendedMode = ovrHmdCap_ExtendDesktop && hmd->HmdCaps;
	int windowPosX = hmd->WindowsPos.x;
	int windowPosY = hmd->WindowsPos.y;

	int resolutionW = hmd->Resolution.w;
	int resolutionH = hmd->Resolution.h;

	if (!glfwInit()) {
		cerr << "Failed to init GLFW." << std::endl;
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(glfwErrorCallback);
	int width = resolutionW, height = resolutionH;
	//Create window based upon setup
	if (extendedMode){
		//Find our monitor
		//see: http://www.glfw.org/docs/latest/rift.html
		int count = 0;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		GLFWmonitor *monitor = nullptr;
#if PLATFORM == PLATFORM_WINDOWS
		for (int i = 0; i < count; i++) {
			//native Win32 function in glfw3native, used on windows to get a monitor
			if (strcmp(glfwGetWin32Monitor(monitors[i]), hmd->DisplayDeviceName) == 0)
				monitor = monitors[i];
		}
#endif
		//provide hints to GLFW
		if (nullptr != monitor) {
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			width = resolutionW;
			height = resolutionH;
		}
		else {
			//TODO: Test on other machines
			width = hmd->Resolution.w;
			height = hmd->Resolution.h;
		}

		//prevent resizing of the HMD by the window manager if it's much larger than the normal monitor
		glfwWindowHint(GLFW_DECORATED, 0);

		//Finally, create and attach the window
		window = glfwCreateWindow(width, height, "test", nullptr, nullptr);
		
		ovrHmd_AttachToWindow(hmd, glfwGetWin32Window(window), nullptr, nullptr);
#ifdef DEBUG
		cout << "DEBUG: Running in extended mode." << std::endl;
#endif
	}
	else {
		//Direct Mode
		//Create the window
		window = glfwCreateWindow(resolutionW, resolutionH, "test", nullptr, nullptr);

		//Direct rendering from a window handle to the Hmd
		ovrHmd_AttachToWindow(hmd, glfwGetWin32Window(window), nullptr, nullptr);
#ifdef DEBUG
		cout << "DEBUG: Running in direct mode." << std::endl;
#endif
	}

	if (!window) {
		glfwTerminate();
		cerr << "ERROR: Couldn't create window" << std::endl;
		exit(EXIT_FAILURE);
	}

	//Setup context and glfw callbacks
	glfwSetWindowPos(window, windowPosX, windowPosY);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	//Init GLEW
	glewInit();
	if (glewIsSupported("GL_VERSION_4_5")) {
#ifdef DEBUG
		std::cout << "YES" << std::endl;
#endif
	}
	else {
#ifdef DEBUG
		std::cout << " NO" << std::endl;
#endif
	}


	//get texture sizes
	ovrSizei eyeSize[2];
	eyeSize[0] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0);
	eyeSize[1] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0);

	//we'll use a single frame buffer
	//Window size bug has been found to be because of eye size being larger than the window size.
	//Window sizes are correct but the framebuffer is larger, h
	/*if (2 * eyeSize[0].w > width) {
		eyeSize[0].w = width/2; eyeSize[1].w = width/2;
	}
	if (eyeSize[1].h > height) {
		eyeSize[0].h = height; eyeSize[1].h = height;
	}*/
	frameBufferWidth = eyeSize[0].w + eyeSize[1].w;
	frameBufferHeight = (eyeSize[0].h > eyeSize[1].h) ? eyeSize[0].h : eyeSize[1].h;
	updateRenderTarget(frameBufferWidth, frameBufferHeight);

	//Configure OpenGL for the OVR
	ovrGLConfig config;
	config.OGL.Header.API = ovrRenderAPI_OpenGL;
	config.OGL.Header.BackBufferSize = Sizei(hmd->Resolution.w, hmd->Resolution.h);
	config.OGL.Header.Multisample = 1;
#ifdef OVR_OS_WIN32
	config.OGL.Window = glfwGetWin32Window(window);
	config.OGL.DC = wglGetCurrentDC();
#endif

	//Update OVR view of render target textures
	for (int i = 0; i < 2; i++) {
		frameBufferOvrTexture[i].OGL.Header.API = ovrRenderAPI_OpenGL;
		frameBufferOvrTexture[i].OGL.Header.TextureSize.w = frameBufferWidth;
		frameBufferOvrTexture[i].OGL.Header.TextureSize.h = frameBufferHeight;
		frameBufferOvrTexture[i].OGL.Header.RenderViewport.Pos.x = (i == 0) ? 0 : frameBufferWidth / 2;
		frameBufferOvrTexture[i].OGL.Header.RenderViewport.Pos.y = 0;
		frameBufferOvrTexture[i].OGL.Header.RenderViewport.Size.w = frameBufferWidth / 2;
		frameBufferOvrTexture[i].OGL.Header.RenderViewport.Size.h = frameBufferHeight;
		frameBufferOvrTexture[i].OGL.TexId = frameBufferTexture; //the eyes use the same texture
	}

	//Enable low-presistence display and dynamic prediction
	unsigned int hmdCaps = ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction;
	ovrHmd_SetEnabledCaps(hmd, hmdCaps);

	//Enable SDK-rendering 
	unsigned int distortionCaps = ovrDistortionCap_Chromatic | ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive;
	ovrBool result = ovrHmd_ConfigureRendering(hmd, &config.Config, distortionCaps, hmd->DefaultEyeFov, eyeRenderDesc);
	if (!result) {
		cerr << "ERROR: failed to configure distortion rendering" << std::endl;
	}

	//Bind the frame buffer as the output one
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

	//Init glut's user input listener
	//glutSpecialFunc(processSpecialKeys);

}

///Test function
void Display::Output()
{
	// Optional: we can overwrite the previous console to more
	// easily see changes in values
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(h, &bufferInfo);

	while (hmd){
		frameTiming = ovrHmd_BeginFrameTiming(hmd, 0);
		ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, frameTiming.ScanoutMidpointSeconds);

		if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
			// The cpp compatibility layer is used to convert ovrPosef to Posef (see OVR_Math.h)
			Posef pose = ts.HeadPose.ThePose;
			float yaw, pitch, roll;
			pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);

			// Optional: move cursor back to starting position and print values
			SetConsoleCursorPosition(h, bufferInfo.dwCursorPosition);
			cout << "yaw: " << RadToDegree(yaw) << endl;
			cout << "pitch: " << RadToDegree(pitch) << endl;
			cout << "roll: " << RadToDegree(roll) << endl;

			Sleep(50);

			if (_kbhit()) exit(0);
		}
		ovrHmd_EndFrameTiming(hmd);
	}
}

void Display::cleanup() {
	//keep this order! See GLFW documentation for integrating with the Rift 
	glfwTerminate();
	ovrHmd_Destroy(hmd);
	ovr_Shutdown();
}

///
/// Creates the needed framebuffers, textures, renderbuffers and sets the needed properties
///
void Display::updateRenderTarget(int width, int height) {
	if (!frameBufferObject) {
		//If no frame buffer, create all objects
		glGenFramebuffers(1, &frameBufferObject);

		//texture for colour buffers
		glGenTextures(1, &frameBufferTexture);

		//render buffer for depth buffer
		glGenRenderbuffers(1, &frameBufferDepth);

		//setup colour buffer filtering
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	//Bind our frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

	//create and attach colour texture 
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameBufferTexture, 0);

	//create and attach render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, frameBufferDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBufferDepth);

	//Check the frame buffer status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		cerr << "ERROR: Couldn't init framebuffer" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef DEBUG
	cout << "Created frame buffer. (" << width << ", " << height << ")" << std::endl;
#endif
}

///
///
///mat is a 4*4 matrix
///
///See: http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/ 
/// for implementation details
///
///quat is i qx + j qy + k qz + qw
///TODO: Make more efficient
///
void Display::convertQuaternionToMatrix(const float* quat, float* mat) {
	mat[0] = 1.0f - 2.0f * quat[1] * quat[1] - 2.0f * quat[2] * quat[2];
	mat[1] = 2.0f * quat[0] * quat[1] - 2.0f * quat[2] * quat[3];
	mat[2] = 2.0f * quat[0] * quat[2] + 2.0f * quat[1] * quat[3];
	mat[3] = 0.0f;

	mat[4] = 2.0f * quat[0] * quat[1] + 2.0f * quat[2] * quat[3];
	mat[5] = 1.0f - 2.0f * quat[0] * quat[0] - 2.0f * quat[2] * quat[2];
	mat[6] = 2.0f * quat[1] * quat[2] - 2.0f * quat[0] * quat[3];
	mat[7] = 0.0f;

	mat[8] = 2.0f * quat[0] * quat[2] - 2.0f * quat[1] * quat[3];
	mat[9] = 2.0f * quat[1] * quat[2] + 2.0f * quat[0] * quat[3];
	mat[10] = 1.0f - 2.0f * quat[0] * quat[0] - 2.0f * quat[1] * quat[1];
	mat[11] = 0.0f;

	mat[12] = mat[13] = mat[14] = 0.0f;
	mat[15] = 1.0f;
}

Display::~Display() {
	cleanup();
}


void Display::run(std::string remote_IP, int remote_port, std::string local_IP, int local_port) {
	//Create objects
	
	try {
		client.setAddresses(Address(local_IP, local_port), Address(remote_IP, remote_port));
	}
	
	catch (...){
		std::cout << "instantiating client failed";
	}
	//try login here
	Serializer serializer;
	int userID;
	bool cont = true;
	int attempts = 0;
	unsigned char *response = new unsigned char[15];
	while (cont && attempts < 5) {
		if (client.sendLoginRequest()) {
			std::cout << "login request sent\n";
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
		if (client.receive(response,20) > 0) {
			std::string message((char *)response);
			std::cout << "received " << message << "\n";
			if (message.compare(0, 14, "LOGIN ACCEPTED") == 0) {
				//successful
				int p = 15;
				userID = serializer.unpackInt(&response[p], p);
				std::cout << "login to game server successful\n";
				cont = false;
			}
		}
		else {
			//timeout and retry
			attempts++;
		}
	}
	if (attempts == 5) {
		std::cout << "Login to game server failed\n";
	}
	
	UpdateManager& updateManager = UpdateManager::getInstance();
	ObjectManager& objectManager = ObjectManager::getInstance();

	float dt = 0.0f;
	double lastTime = glfwGetTime();
	int numFrames = 0;
	int count = 0;
	//game loop
	while (!glfwWindowShouldClose(window)) {

		//Time to draw a frame, averaged over one second
		double currentTime = glfwGetTime();
		numFrames++;
		if (currentTime - lastTime >= 1.0) {
			//If at least one second passed
			std::cout << "FPS: " << numFrames << ". " << (1000.0 / (double)numFrames) << " ms/frame\n" << std::endl;
			numFrames = 0;
			lastTime += 1.0;
		}

		//AT THE MOMENT THIS SENDS MESSAGES TOO FAST FOR THE SERVER TO HANDLE QUICKLY ENOUGH
		//client.sendPitchRollYaw(getHeadOrientation());

		
		//networking get updates;
		unsigned char *buffer = new unsigned char[1024];
		int updateSize;
		if ((updateSize = client.receive(buffer,1024)) >= 0) {
			//std::cout << "received update from server, size " << updateSize << "\n";
			int pos = 0;
			while (pos < updateSize) {
				int temp;
				int globalID = serializer.unpackInt(&buffer[pos+2],temp);
				int localID = objectManager.getObjectLocalFromGlobalID(globalID);
				bool toDelete = false;
				if (buffer[pos + 1] == 'D') {
					toDelete = true;
				}
				if (buffer[pos] == 'P') {
					//player object
					std::shared_ptr<Player> player;
					if (objectManager.exists(localID)) {
						player = std::dynamic_pointer_cast<Player>(objectManager.getObject(localID));
					}
					else {
						float x = 0.0f, y = 50.0f, z = 0.0f;
						player = std::make_shared<Player>(x,y,z);
						player->setGlobalID(globalID);
						objectManager.setObjectGlobal(globalID, objectManager.getNextID());
						player->setID(objectManager.getObjectLocalFromGlobalID(globalID));
						//If this is the current player
						if (globalID == userID) {
							//TODO: Remove this HACK
							Display* disp = (Display*)this;
							(dynamic_cast<Game*>(disp))->player = player;
						}
					}
					pos += 2;
					if (!toDelete) {
						pos += player->deserialize(&buffer[pos]);
						if (!objectManager.exists(localID)) {
							objectManager.addObject(player);
						}
					}
					else {
						objectManager.removeObject(localID);
					}

				}
				else {
					//game object
					std::shared_ptr<GameObject> object;
					if (objectManager.exists(localID)) {
						object = objectManager.getObject(localID);
					}
					else {
						objectManager.setObjectGlobal(globalID, objectManager.getNextID());
						object = std::make_shared<GameObject>();
						object->setGlobalID(globalID);
						object->setID(objectManager.getObjectLocalFromGlobalID(globalID));
					}
					pos += 2;
					if (!toDelete) {
						pos += object->deserialize(&buffer[pos]);
						if (!objectManager.exists(localID)) {
							objectManager.addObject(object);
						}
					}
					else {
						objectManager.removeObject(localID);
					}
				}
			}

			//updateManager.processUpdates()
		}
		else {
			//some form of prediction
		}
		
		//physics tick 
		//TODO calculate dt per frame
		//simulator.tick(dt);

		//send updates to server
		//client.send(); // maybe called by updatemanager

		//send PRY
		count++;
		if (count % 10 == 0) {
			client.sendPitchRollYaw(getHeadOrientation());
			count = 0;
		}
		//render game
		//TODO: REMOVE THIS HACK TOO
		Display* disp = (Display*)this;
		Game *g = (dynamic_cast<Game*>(disp));
		if (g->getPlayer()) {
			render();
		}
		else {
			std::cout << "Player not instantiated yet \n";
		}
	}
}

void Display::findHeadOrientation() {
	ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, frameTiming.ScanoutMidpointSeconds);

	if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
		// The cpp compatibility layer is used to convert ovrPosef to Posef (see OVR_Math.h)
		Posef pose = ts.HeadPose.ThePose;
		float yaw, pitch, roll;
		pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);
		headOrientation = glm::vec3(pitch, roll, yaw);
		//std::cout << "YAW: " << yaw << " PITCH " << pitch << " ROLL " << roll << std::endl;
	}
}
void Display::render() {

	//Start a frame
	ovrFrameTiming hmdFrameTiming = ovrHmd_BeginFrame(hmd, 0);
		
	//Bind in the frame buffer for the HMD
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

	//Get head tracking info
	findHeadOrientation();

	//Base OpenGL
	glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f/ 255.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	ovrPosef headPose[2];
	//For each eye
	for (int eyeIndex = 0; eyeIndex < 2; eyeIndex++) {
		//The current eye, use this not just assume left then right as this can
		//improve performance
		ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];

		//View port transform
		//we draw in the left half of framebuffer for left eye view and in the right
		//for the right eye
		glViewport(eye == ovrEye_Left ? 0 : frameBufferWidth / 2,
			0,
			frameBufferWidth / 2,
			frameBufferHeight);

		//Projection matrix
		//OVR matrices are the transpose of what OpenGL wants so we need to load the transpose.
		Matrix4f proj = ovrMatrix4f_Projection(eyeRenderDesc[eye].Fov, 0.01f, 1000.0f, true);
		glm::mat4 projectionMatrixPreTrans = glm::make_mat4(proj.M[0]);
		glm::mat4 projectionMatrix = glm::transpose(projectionMatrixPreTrans);

		//View matrix
		//position of eyes
		headPose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);
		glm::mat4 modelView = glm::mat4(1.0f);

		// Inter-pupil Distance in meters, need to translate the camera by this distance
		modelView = glm::translate(modelView, 
			glm::vec3(eyeRenderDesc[eye].HmdToEyeViewOffset.x,
			eyeRenderDesc[eye].HmdToEyeViewOffset.y,
			eyeRenderDesc[eye].HmdToEyeViewOffset.z));
	
		//orientation of the headset
		//multiply the view matrix by this rotation
		const float orientationQuat[4] = {
			headPose[eye].Orientation.x,
			headPose[eye].Orientation.y,
			headPose[eye].Orientation.z,
			headPose[eye].Orientation.w };

		float orientationMatrix[16];
		convertQuaternionToMatrix(orientationQuat, orientationMatrix);
		modelView = glm::make_mat4(orientationMatrix) * modelView;

		//Move camera to eye level
		modelView = glm::translate(modelView, glm::vec3(0.0f, -ovrHmd_GetFloat(hmd, OVR_KEY_EYE_HEIGHT, 1.65f),0.0f));

		//RENDER
		renderScene(modelView, projectionMatrix);
		
	}
	glWindowPos2i(100, 120);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	std::string str = "TEsting the new text engine!!";
	const unsigned char * strP = (const unsigned char*)str.c_str();
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, strP);
	glWindowPos2i(1400, 120);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, strP);

	//revert to drawing to display
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Let the Rift handle lens distortion, chromatic abberation et.c
	ovrHmd_EndFrame(hmd, headPose, &frameBufferOvrTexture[0].Texture);

	//Fix sdk renderer bug, uses shader but doesn't restore bindings
	glUseProgram(0);

	//For debugging purposes
	//assert(glGetError() == GL_NO_ERROR);

	glfwPollEvents();


	//Don't need to swap buffers, handled by the SDK

}

