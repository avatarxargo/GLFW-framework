///=========================================
///				computeShader.h
///	@author:	David Hrusa
///	@email:		hrusa@pansyn.com
///	@date:		12/3/2019
/// @desc:		creates and maintains a compute shader
///
///=========================================

#ifndef __COMPUTE_SHADER_H__
#define __COMPUTE_SHADER_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include "../rika/display/ShaderComponents/ShaderCore.h"

namespace toolkit {

	struct ComputeShader {
		std::string path;
		int x = 1;
		int x_groups = 1;
		int y = 1;
		int y_groups = 1;
		int z = 1;
		int z_groups = 1;
		GLuint programID;
		GLuint shaderID;
		GLuint inBufferA, inBufferB, outBuffer;
	};

	ComputeShader createComputeShader(std::string path);
	void dispatchComputeShader(ComputeShader *shader);
}

#endif