///=========================================
///				toolkit.h
///	@author:	David Hrusa
///	@email:		hrusa@pansyn.com
///	@date:		11/19/2019
/// @desc:		includes the rest of the gui framework
///
///=========================================

#ifndef __TOOLKIT_H__
#define __TOOLKIT_H__

#include "globals/errorCodes.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

namespace toolkit {

	/// Sets up the drawing framework
	void initToolkit(void(*upd_function)(double), void(render_function)(void));

	/// Sets everything up, so that 
	void initImGui();

	/// Dispalys information about the GPU
	void printGlInfo();

	/// Destroys everything that keeps this framework running.
	void cleanUp();

	/// Starts updating the application
	void startMainLoop();

	GLFWwindow* const getWindow();

	void error_callback(int error, const char* description);
}

#endif