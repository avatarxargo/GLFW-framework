/**
 * Keeps track of both keyboard and controlers.
 *
 * @date: 4/5/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "InputHandler.h"

namespace InputHandler {
	int playerCount = 0;
	int playerId = 0;
	glm::vec2 mousePos;
	glm::vec2 dragLastPos;
	std::map<int, PlayerInput> playerInputs;
	GameWindow* window;
	//
	bool dragMode = false;
	bool dragModeBegin = false;
	glm::vec2 dragDelta;
	float scrollDelta;
}

bool InputHandler::keyPressed(int playerId, GameInput key) {
	switch (key) {
	case LEFT:
		return playerInputs[playerId].inputMap.left;
	case RIGHT:
		return playerInputs[playerId].inputMap.right;
	case UP:
		return playerInputs[playerId].inputMap.up;
	case DOWN:
		return playerInputs[playerId].inputMap.down;
	case A:
		return playerInputs[playerId].inputMap.A;
	case B:
		return playerInputs[playerId].inputMap.B;
	case C:
		return playerInputs[playerId].inputMap.C;
	case D:
		return playerInputs[playerId].inputMap.D;
	default:
		return false;
	}
}

bool InputHandler::updatePlayerInputs(int key, int action) {
	int hit = 0;
	for (int i = 0; i < playerId; ++i) {
		KeyboardScheme* keyboardScheme = &(playerInputs[i].keyboard);
		InputMap* inputMap = &(playerInputs[i].inputMap);
		if (key == keyboardScheme->A) {
			inputMap->left = (action != GLFW_RELEASE);
		}
		else if (key == keyboardScheme->W) {
			inputMap->up = (action != GLFW_RELEASE);
		}
		else if (key == keyboardScheme->S) {
			inputMap->down = (action != GLFW_RELEASE);
		}
		else if (key == keyboardScheme->D) {
			inputMap->right = (action != GLFW_RELEASE);
		}
		else if (key == keyboardScheme->Z) {
			inputMap->A = (action != GLFW_RELEASE);
		}
		else if (key == keyboardScheme->X) {
			inputMap->B = (action != GLFW_RELEASE);
		}
		else if (key == keyboardScheme->C) {
			inputMap->C = (action != GLFW_RELEASE);
		}
		else if (key == keyboardScheme->Shift) {
			inputMap->D = (action != GLFW_RELEASE);
		}
		else {
			hit++;
		}
	}
	//returns true if each player profile went through its keybindings and did not find a hit.
	return playerInputs.size() == hit;
}

void InputHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(updatePlayerInputs(key, action)) {
		std::fprintf(stderr, "unmapped key %d pressed\n", key);
	}
}

void InputHandler::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mousePos.x = (float)xpos;
	mousePos.y = 720-(float)ypos;
	if (dragMode) {
		dragDelta += mousePos - dragLastPos;
		dragLastPos = mousePos;
	}
}

void InputHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//begin drag
		InputHandler::setDragMode(true);
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		//end drag
		InputHandler::setDragMode(false);
	}
}

void InputHandler::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	InputHandler::scrollDelta += yoffset;
}

void InputHandler::init(GameWindow* _gameWindow) {
	window = _gameWindow;
	glfwSetKeyCallback(window->getGLFWWindow(), key_callback);
	glfwSetCursorPosCallback(window->getGLFWWindow(), cursor_position_callback);
	glfwSetMouseButtonCallback(window->getGLFWWindow(), mouse_button_callback);
	glfwSetScrollCallback(window->getGLFWWindow(), scroll_callback);
}

int InputHandler::registerPlayer(char* name) {
	PlayerInput newPlayer;
	newPlayer.name = name;
	int newId = playerId++;
	playerInputs.insert( std::pair<int,PlayerInput>(newId,newPlayer) );
	return newId;
}

void InputHandler::setDragMode(bool mode) {
	dragMode = mode;
	if (dragMode) {
		dragModeBegin = true;
		dragLastPos = mousePos;
	} else {
		dragModeBegin = false;
		clearDragDelta();
	}
}

bool InputHandler::isDragMode() {
	return dragMode;
}

bool InputHandler::isDragModeBegin() {
	bool ret = dragModeBegin;
	dragModeBegin = false;
	return ret;
}

glm::vec2* InputHandler::getDragDelta() {
	return &dragDelta;
}

void InputHandler::clearDragDelta() {
	dragDelta.x = 0;
	dragDelta.y = 0;
}

float InputHandler::getSrollDelta() {
	return scrollDelta;
}

void InputHandler::clearSrollDelta() {
	scrollDelta = 0;
}

glm::vec2* InputHandler::getMousePos() {
	return &mousePos;
}

InputHandler::PlayerInput* InputHandler::getPlayerInput(int idx) {
	return &(playerInputs[idx]);
}