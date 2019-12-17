///=========================================
///				computeShader.cpp
///	@author:	David Hrusa
///	@email:		hrusa@pansyn.com
///	@date:		12/3/2019
/// @desc:		creates and maintains a compute shader
///
///=========================================

#include "computeShader.h"

namespace toolkit {
	ComputeShader createComputeShader(std::string path) {
		ComputeShader shader;
		shader.path = path;
		// Compile the program:
		shader.programID = glCreateProgram();
		if (loadShader(GL_COMPUTE_SHADER, path.c_str(), &shader.shaderID)) {
			std::cout << "Trouble loading Compute Shader.\n";
		}
		glCompileShader(shader.shaderID);
		int rvalue;
		glGetShaderiv(shader.shaderID, GL_COMPILE_STATUS, &rvalue);
		if (!rvalue) {
			fprintf(stderr, "Error in compiling the compute shader\n");
			GLchar log[10240];
			GLsizei length;
			glGetShaderInfoLog(shader.shaderID, 10239, &length, log);
			fprintf(stderr, "Compiler log:\n%s\n", log);
			//exit(40);
		}
		glAttachShader(shader.programID, shader.shaderID);
		//
		glLinkProgram(shader.programID);
		glGetProgramiv(shader.programID, GL_LINK_STATUS, &rvalue);
		if (!rvalue) {
			fprintf(stderr, "Error in linking compute shader program\n");
			GLchar log[10240];
			GLsizei length;
			glGetProgramInfoLog(shader.programID, 10239, &length, log);
			fprintf(stderr, "Linker log:\n%s\n", log);
			//exit(41);
		}
		// Set up buffers:
		//A
		glGenBuffers(1, &shader.inBufferA);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, shader.inBufferA);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 0, 0, GL_DYNAMIC_STORAGE_BIT);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shader.inBufferA);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		//B
		/*glGenBuffers(1, &shader.inBufferB);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, shader.inBufferB);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, shader.inBufferB);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);*/
		//out
		glGenBuffers(1, &shader.outBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, shader.outBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 0, 0, GL_DYNAMIC_STORAGE_BIT);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, shader.outBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		return shader;
	}

	void dispatchComputeShader(ComputeShader *shader) {
		glDispatchCompute(shader->x / shader->x_groups,
						  shader->y / shader->y_groups,
						  shader->z / shader->z_groups);
	}
}