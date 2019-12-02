///=========================================
///				Camera.h
///	@author:	David Hrusa
///	@email:		hrusa@pansyn.com
///	@date:		12/1/2019
/// @desc:		Holds information about the camera and latest matrices
///
///=========================================

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace toolkit {

	struct Camera {
		float campos[3] = { 0.0f,0.0f,0.0f };
		float camstats[3] = { 40.0f,0.01f,100.0f };
		float camangle = 0.0f;
		float campitch = 0.0f;
		float camdst = 5.0f;
		glm::mat4 PVM;
		glm::mat4 P;
		glm::mat4 V;
		glm::mat4 M;
		glm::mat4 N;
	};

}

#endif