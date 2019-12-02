/**
 * Maintains FBOs and multipas render target.
 *
 * @date: 4/24/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef FBO_HANDLER_H
#define FBO_HANDLER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderComponents/ShaderCore.h"

namespace fboShaAtt {

	// attribute locations for the FBO 2nds pass Renderer

	const int pos = 0;
	const int uv = 1;
}

namespace fboShaUni {

	// uniform locations for the FBO 2nds pass Renderer

	const int fbotex1 = 2;
	const int fbotex2 = 3;
	const int fbotex3 = 4;
}

class FBOHandler : ShaderCore
{
private:
	const char* VERT_PATH = "data/shaders/fbo_vert.glsl";
	const char* GEOM_PATH = "";
	const char* FRAG_PATH = "data/shaders/fbo_frag.glsl";
	GLuint vao;
	ContainerVBO vbo;
	glm::mat4 mvp;
	//fbo
	int fbo_texture_width = 1280;
	int fbo_texture_height = 720;
	GLuint fbo; //fbo
	GLuint fborb; //render buffer
public:
	GLuint fbotex1, fbotex2, fbotex3; //attachment0
	bool init();
	bool createVAO();
	void bind();
	void unbind();
	void clear();
	void drawPass2(glm::vec2 screensize);
	//
	FBOHandler();
	~FBOHandler();
};

#endif