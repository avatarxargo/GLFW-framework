///=========================================
///				launcher.cpp
///	@author:	David Hrusa
///	@email:		hrusa@pansyn.com
///	@date:		11/19/2019
/// @desc:		launches an instance of the toolkit
///
///=========================================

#include "../toolkit/toolkit.h"
#include "../toolkit/data/Mesh.h"
#include "scene.h"
#include <iostream>

using namespace toolkit;

void update(double delta) {
	updateScene(delta);
}

void render() {
	renderScene();
}

int main(int argc, char** args) {
	initToolkit(update,render);
	createScene();
	startMainLoop();
	return TLKT_EXIT_CODE_NOPROBLEM;
}