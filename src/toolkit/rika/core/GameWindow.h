/**
 * The display window class. Manages input handling + rendering.
 * The rest of the game systems should mainly not be aware of the rendering stuff.
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "Constants/Constants.h"
#include <stdio.h>
#include <FreeImage.h>
#include "RikaGameInterface.h"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
#include "../display/Camera.h"

//Forward declare the main game class.

class GameWindow : public GameWindowInterface {
private:
	RikaGameInterface* game;
	GLFWwindow* window;
	ShaderContainer shaderContainer;
	Camera* camera;
public:
	/**
	 * Creates a starting window and context.
	 * @return true if the initialization failed.
	 */
	bool init(RikaGameInterface* game);

	// ========================================= Interfaced =========================================

	/**
	 * @updRatio - 0-1 ratio showing the portion of time elapsed towards the next update tick. Used to interpolate visuals.
	 */
	void render1st(double updRatio);
	void render2nd(double updRatio);
	void reloadShaders();
	void closeWindow();
	//getters and setter
	RikaGameInterface* getGame();
	ShaderContainer getShaders();
	GLFWwindow* getGLFWWindow();
	Camera* getCamera();
};

#endif
