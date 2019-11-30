///=========================================
///				toolkit.cpp
///	@author:	David Hrusa
///	@email:		hrusa@pansyn.com
///	@date:		11/19/2019
/// @desc:		includes the rest of the gui framework
///
///=========================================

#include "toolkit.h"

GLFWwindow* window;

void(*ref_upd_function)(double);
void(*ref_render_function)(void);

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void toolkit::printGlInfo()
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void toolkit::initToolkit(void(*upd_function)(double), void(render_function)(void)) {
	// store functions
	ref_upd_function = upd_function;
	ref_render_function = render_function;
	//
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		// Initialization failed
	}
	// Require minimal version:
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(1200, 800, "My Title", NULL, NULL);
	if (!window)
	{
		// Window or OpenGL context creation failed
	}
	glfwMakeContextCurrent(window);
	glewInit();
	printGlInfo();
	initImGui();
}

void toolkit::initImGui() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);
	io.Fonts->AddFontFromFileTTF("data/fonts/ltromatic.ttf", 20.0f);
}

void toolkit::startMainLoop() {
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Keep running
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glfwPollEvents();
		double time = glfwGetTime();
		glViewport(0, 0, width, height);
		ref_upd_function(time);
		ref_render_function();
		glfwSwapBuffers(window);
	}
	
	// Clean up
	cleanUp();
}

void toolkit::cleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* const toolkit::getWindow() {
	return window;
}