/**
 * Allows access to the game window.
 *
 * @date: 3/29/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef GAMEWINDOWINTERFACE_H
#define GAMEWINDOWINTERFACE_H

#include "../display/Shaders/SpriteRenderer.h"
#include "../Display/Shaders/CircleRenderer.h"
#include "../Display/Shaders/BoneRenderer.h"
#include "../Display/Shaders/BezierRenderer.h"
#include "../Display/Shaders/LineRenderer.h"
#include "../Display/Shaders/SimplePolyRenderer.h"
#include "../display/FBOHandler.h"

struct ShaderContainer {
	SpriteRenderer* spriteRenderer;
	CircleRenderer* circleRenderer;
	BoneRenderer* boneRenderer;
	BezierRenderer* bezierRenderer;
	LineRenderer* lineRenderer;
	SimplePolyRenderer* simplePolyRenderer;
	//
	FBOHandler* fboHandler;
};

class GameWindowInterface {
public:
	virtual void render1st(double updRatio) = 0;
	virtual void render2nd(double updRatio) = 0;
	virtual void reloadShaders() = 0;
	virtual void closeWindow() = 0;
	//getters and setter
	//virtual RikaGameInterface* getGame() = 0;
	virtual ShaderContainer getShaders() = 0;
	virtual GLFWwindow* getGLFWWindow() = 0;
	virtual Camera* getCamera() = 0;
};

#endif
