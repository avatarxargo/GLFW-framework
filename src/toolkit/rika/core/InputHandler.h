/**
 * Keeps track of both keyboard and controlers.
 *
 * @date: 4/5/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "GameWindow.h"

namespace InputHandler {

	enum GameInput {
		UP, LEFT, DOWN, RIGHT, A, B, C, D
	};

	struct KeyboardScheme {
		int A;
		int W;
		int S;
		int D;
		int Z;
		int X;
		int C;
		int Shift;
	};

	struct ControllerScheme {
		char Dup;
		char Dleft;
		char Dright;
		char Ddown;
		char A;
		char B;
		char C;
		char D;
		char L1;
		char L2;
		char R1;
		char R2;
	};

	struct InputMap {
		bool left = false;
		bool right = false;
		bool up = false;
		bool down = false;
		bool A = false;
		bool B = false;
		bool C = false;
		bool D = false;
		bool L1 = false;
		bool L2 = false;
		bool R1 = false;
		bool R2 = false;
	};

	struct PlayerInput {
		char* name;
		KeyboardScheme keyboard;
		ControllerScheme controller;
		InputMap inputMap;
	};

	bool keyPressed(int playerId, GameInput key);
	bool updatePlayerInputs(int key, int action);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void init(GameWindow* _gameWindow);
	int registerPlayer(char* name);
	void setDragMode(bool mode);
	bool isDragMode();
	bool isDragModeBegin();
	glm::vec2* getDragDelta();
	void clearDragDelta();
	float getSrollDelta();
	void clearSrollDelta();
	glm::vec2* getMousePos();
	PlayerInput* getPlayerInput(int idx);
}

#endif
