/**
 * Handles the shader for rendering sprites.
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

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

namespace sprShaAtt {

	// attribute locations for the Sprite Renderer

	const int pos = 0;
	const int size = 1;
	const int uvstart = 2;
	const int uvdim = 3;
	const int texid = 4;
	const int highlight = 5;
	const int angle = 6;
	const int color = 7;
}

namespace sprShaUni {

	// uniform locations for the Sprite Renderer

	const int mvp = 0;
	const int screensize = 1;
}

struct Sprite {
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 uvstart;
	glm::vec2 uvdim;
	float texid;
	float highlight = 0;
	float angle;
};

class SpriteRenderer : public ShaderCore
{
private:
	const char* VERT_PATH = "data/shaders/sprite_vert.glsl";
	const char* GEOM_PATH = "data/shaders/sprite_geom.glsl";
	const char* FRAG_PATH = "data/shaders/sprite_frag.glsl";
	GLuint vao;
	ContainerVBO vbo;
	int count = 0;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 mvp;
public:
	int cammode = 0;
	TextureImage tex1;
	TextureImage tex2;
	TextureImage tex3;
	TextureImage tex4;
	bool init();
	bool createVAO();
	/**
	 * Transfers the host data to the device.
	 */
	void pushToDevice();
	void insertSprite(UISprite* sprite);
	void insertSprite(Sprite sprite);
	void insertSprite(glm::vec2 pos, glm::vec2 size, glm::vec2 uvstart, glm::vec2 uvdim, float texid, float highlight, float angle);
	void insertHandler(UIHandler* handler);
	void clearVBO();
	void deleteShaders();
	void draw(glm::vec2 screensize, Camera* camera);
	//
	SpriteRenderer();
	~SpriteRenderer();
};

#endif