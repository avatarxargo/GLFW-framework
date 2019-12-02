/**
 *
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "GameWindow.h"

//TODO: figure out how to make this non-static.
static GameWindow* me;

void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_R) {
			me->reloadShaders();
		}
	}
}

static void close_callback(GLFWwindow* window) {
	fprintf(stderr, "Close\n");
	me->getGame()->closeCallback();
}

bool GameWindow::init(RikaGameInterface* game) {
	// Store callback functions
	me = this;
	this->game = game;
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Fatal Error: Initializing GLFW failed!\n");
		return true;
	}
	// Set callbacks
	glfwSetErrorCallback(error_callback);
	// Create the window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(1280, 720, "Rika Engine ver 1.0a", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "Fatal Error: Creating Window or OpenGL failed!\n");
		return true;
	}
	glfwSetWindowPos(window, 600,120);
	// Some window specific callbacks
	glfwSetWindowCloseCallback(window, close_callback);
	glfwSetKeyCallback(window, key_callback);
	// Connecting the window to the GLcontext
	glfwMakeContextCurrent(window);
	// Load OpenGL extensions
	GLuint err = glewInit();
	if (err) {
		fprintf(stderr, "Fatal Error: Loading GLEW failed! %s\n", glewGetErrorString(err));
		return true;
	}
	else {
		printf("\nGLEW initialized correctly\nGLFW: %s\nGL: %s\n\n%s\n", glfwGetVersionString(), glGetString(GL_VERSION), globals::LINE);
	}
	// vsynch
	glfwSwapInterval(0);
	// window icon
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(game->getWindowIconPath(), 0);
	FIBITMAP *bitmap = FreeImage_Load(formato, game->getWindowIconPath());
	int w = FreeImage_GetWidth(bitmap);
	int h = FreeImage_GetHeight(bitmap);
	BYTE* temp = FreeImage_GetBits(bitmap);
	FreeImage_Unload(bitmap);
	unsigned char iconimagech[133*133*4];
	int offset = 0;// 94;
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			int idx = (i * w + j);
			int idx2 = offset + ((h-1-i) * w + j);
			iconimagech[idx * 4] = (((unsigned char*)temp)[idx2 * 4 + 2]);
			iconimagech[idx * 4 + 1] = (((unsigned char*)temp)[idx2 * 4 + 1]);
			iconimagech[idx * 4 + 2] = (((unsigned char*)temp)[idx2 * 4 + 0]);
			iconimagech[idx * 4 + 3] = 255;
		}
	}
	GLFWimage iconimage = { w,h, iconimagech };
	glfwSetWindowIcon(window,1,&iconimage);
	// Create window shaders
	shaderContainer.spriteRenderer = new SpriteRenderer();
	if (shaderContainer.spriteRenderer->init()) {
		fprintf(stderr, "Error: Sprite renderer initialization failed!\n");
		return true;
	}
	shaderContainer.spriteRenderer->createVAO();
	//
	shaderContainer.circleRenderer = new CircleRenderer();
	if (shaderContainer.circleRenderer->init()) {
		fprintf(stderr, "Error: Circle renderer initialization failed!\n");
		return true;
	}
	shaderContainer.circleRenderer->createVAO();
	//
	shaderContainer.boneRenderer = new BoneRenderer();
	if (shaderContainer.boneRenderer->init()) {
		fprintf(stderr, "Error: Bone renderer initialization failed!\n");
		return true;
	}
	shaderContainer.boneRenderer->createVAO();
	//
	shaderContainer.bezierRenderer = new BezierRenderer();
	if (shaderContainer.bezierRenderer->init()) {
		fprintf(stderr, "Error: Bezier renderer initialization failed!\n");
		return true;
	}
	shaderContainer.bezierRenderer->createVAO();
	//
	/*shaderContainer.lineRenderer = new LineRenderer();
	if (shaderContainer.lineRenderer->init()) {
		fprintf(stderr, "Error: Line renderer initialization failed!\n");
		return true;
	}
	shaderContainer.lineRenderer->createVAO();
	*///
	shaderContainer.fboHandler = new FBOHandler();
	if (shaderContainer.fboHandler->init()) {
		fprintf(stderr, "Error: FBO handler initialization failed!\n");
		return true;
	}
	//
	shaderContainer.simplePolyRenderer = new SimplePolyRenderer();
	if (shaderContainer.simplePolyRenderer->init()) {
		fprintf(stderr, "Error: simple poly renderer initialization failed!\n");
		return true;
	}
	//
	camera = new Camera();
	return false;
}

void GameWindow::render1st(double updRatio) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	//shaderContainer.fboHandler->clear();
	shaderContainer.fboHandler->bind();

	//glClearColor(0.3f, 0, 0.4f, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	GLuint clearColor[4] = { 0, 0, 0, 0 };
	glClearBufferuiv(GL_COLOR, 0, clearColor);
	glClearBufferuiv(GL_COLOR, 1, clearColor);
	glClearBufferuiv(GL_COLOR, 2, clearColor);
	//
	shaderContainer.spriteRenderer->draw(glm::vec2(width, height), getCamera());
	shaderContainer.circleRenderer->draw(glm::vec2(width, height));
	shaderContainer.boneRenderer->draw(glm::vec2(width, height));
	shaderContainer.bezierRenderer->draw(glm::vec2(width, height));
	//
	shaderContainer.fboHandler->unbind();
	//glfwSwapBuffers(window);
	//glfwPollEvents();
}

void GameWindow::render2nd(double updRatio) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	shaderContainer.fboHandler->drawPass2(glm::vec2(width,height));
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GameWindow::reloadShaders() {
	printf("Regenerating Shaders...\n");
	shaderContainer.spriteRenderer->deleteShaders();
	shaderContainer.circleRenderer->deleteShaders();
	shaderContainer.boneRenderer->deleteShaders();
	if (shaderContainer.spriteRenderer->init())
		return;
	if (shaderContainer.circleRenderer->init())
		return;
	if (shaderContainer.boneRenderer->init())
		return;
	printf("success\n");
}

void GameWindow::closeWindow() {
	glfwDestroyWindow(window);
}


RikaGameInterface* GameWindow::getGame() {
	return game;
}

ShaderContainer GameWindow::getShaders() {
	return shaderContainer;
}

GLFWwindow* GameWindow::getGLFWWindow() {
	return window;
}

Camera* GameWindow::getCamera() {
	return camera;
}
