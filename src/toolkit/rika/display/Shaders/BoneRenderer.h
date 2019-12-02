/**
 * Handles the shader for rendering character bones.
 *
 * @date: 4/8/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef BONE_RENDERER_H
#define BONE_RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../TextureImage.h"
#include "../ShaderComponents/ContainerVBO.h"
#include "../ShaderComponents/ShaderCore.h"
#include "../UI/UISprite.h"
#include "../Characters/CharacterTools.h"

namespace boneShaAtt {

	// attribute locations for the Circle Renderer

	const int pos1 = 0; //position of the first node
	const int pos2 = 1; //position of the second node
	const int highlight = 2; //vec4 in case we want to pass more effects later
	const int color = 3; //vec4
	const int transform = 4;
	//5
	//6
	//7
}

namespace boneShaUni {

	// uniform locations for the Circle Renderer

	const int mvp = 0;
	const int screensize = 1;
}

class BoneRenderer : public ShaderCore
{
private:
	const char* VERT_PATH = "data/shaders/bone_vert.glsl";
	const char* GEOM_PATH = "data/shaders/bone_geom.glsl";
	const char* FRAG_PATH = "data/shaders/bone_frag.glsl";
	GLuint vao;
	ContainerVBO vbo;
	int count = 0;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 mvp;
	TextureImage tex1;
	TextureImage tex2;
public:
	bool init();
	bool createVAO();
	/**
	 * Transfers the host data to the device.
	 */
	void pushToDevice();
	void insertBone(CharacterTools::Bone* bone);
	void insertSkeleton(CharacterTools::Skeleton* skeleton);
	void clearVBO();
	void deleteShaders();
	void draw(glm::vec2 screensize);
	//
	BoneRenderer();
	~BoneRenderer();
};

#endif