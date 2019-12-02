#include "SpriteRenderer.h"

bool SpriteRenderer::createVAO() {
	// TODO: error handling
	// Bind prog
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//
	vbo.create();
	vbo.bind();
	// glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STREAM_DRAW);
	// Set attributes
	int len = 15;
	glEnableVertexAttribArray(sprShaAtt::pos);
	glVertexAttribPointer(sprShaAtt::pos, 2, GL_FLOAT, false, len * sizeof(float), 0);
	glEnableVertexAttribArray(sprShaAtt::size);
	glVertexAttribPointer(sprShaAtt::size, 2, GL_FLOAT, false, len * sizeof(float), (void*)(2 * sizeof(float)) );
	glEnableVertexAttribArray(sprShaAtt::uvstart);
	glVertexAttribPointer(sprShaAtt::uvstart, 2, GL_FLOAT, false, len * sizeof(float), (void*)(4 * sizeof(float)) );
	glEnableVertexAttribArray(sprShaAtt::uvdim);
	glVertexAttribPointer(sprShaAtt::uvdim, 2, GL_FLOAT, false, len * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(sprShaAtt::texid);
	glVertexAttribPointer(sprShaAtt::texid, 1, GL_FLOAT, false, len * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(sprShaAtt::highlight);
	glVertexAttribPointer(sprShaAtt::highlight, 1, GL_FLOAT, false, len * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(sprShaAtt::angle);
	glVertexAttribPointer(sprShaAtt::angle, 1, GL_FLOAT, false, len * sizeof(float), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(sprShaAtt::color);
	glVertexAttribPointer(sprShaAtt::color, 4, GL_FLOAT, false, len * sizeof(float), (void*)(11 * sizeof(float)));
	// gen Textures for testing
	tex1.load("data/img/rika.png", true);
	tex2.load("data/img/testchamber.png", true);
	// Set uniforms
	//glUseProgram(shaderProgram);
	bindProgram();
	model = glm::mat4(1.0);
	view = glm::mat4(1.0);
	proj = glm::ortho(0.0f,1200.0f,0.0f,800.0f,-1.0f,1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(sprShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	//
	GLuint uniTex1 = glGetUniformLocation(getProgramId(), "texImage1");
	glUniform1i(uniTex1, 0);
	GLuint uniTex2 = glGetUniformLocation(getProgramId(), "texImage2");
	glUniform1i(uniTex2, 1);
	GLuint uniTex3 = glGetUniformLocation(getProgramId(), "texImage3");
	glUniform1i(uniTex3, 2);
	GLuint uniTex4 = glGetUniformLocation(getProgramId(), "texImage4");
	glUniform1i(uniTex4, 3);
	glUseProgram(0);
	// Unbind everything (Let's be respectable of other shaders)
	glBindVertexArray(0);
	return false;
}

void SpriteRenderer::insertSprite(UISprite* sprite) {
	vbo.insert(sprite->position.x);
	vbo.insert(sprite->position.y);
	vbo.insert(sprite->size.x);
	vbo.insert(sprite->size.y);
	vbo.insert(sprite->uvstart.x);
	vbo.insert(sprite->uvstart.y);
	vbo.insert(sprite->uvsize.x);
	vbo.insert(sprite->uvsize.y);
	vbo.insert((float)sprite->texid);
	vbo.insert((float)sprite->highlight);
	vbo.insert(sprite->angle);
	vbo.insert(sprite->color);
	count++;
}

void SpriteRenderer::insertSprite(Sprite sprite) {
	vbo.insert(sprite.position.x);
	vbo.insert(sprite.position.y);
	vbo.insert(sprite.size.x);
	vbo.insert(sprite.size.y);
	vbo.insert(sprite.uvstart.x);
	vbo.insert(sprite.uvstart.y);
	vbo.insert(sprite.uvdim.x);
	vbo.insert(sprite.uvdim.y);
	vbo.insert(sprite.texid);
	vbo.insert(sprite.highlight);
	vbo.insert(sprite.angle);
	vbo.insert(0);
	vbo.insert(0);
	vbo.insert(0);
	vbo.insert(0);
	count++;
}

void SpriteRenderer::insertSprite(glm::vec2 pos, glm::vec2 size, glm::vec2 uvstart, glm::vec2 uvdim, float texid, float highlight, float angle) {
	vbo.insert(pos.x);
	vbo.insert(pos.y);
	vbo.insert(size.x);
	vbo.insert(size.y);
	vbo.insert(uvstart.x);
	vbo.insert(uvstart.y);
	vbo.insert(uvdim.x);
	vbo.insert(uvdim.y);
	vbo.insert(texid);
	vbo.insert(highlight);
	vbo.insert(angle);
	vbo.insert(glm::vec4(1,0,0,0));
	count++;
}

void SpriteRenderer::insertHandler(UIHandler* handler) {
	for (int i = 0; i < handler->sprites.size(); ++i) {
		insertSprite(handler->sprites[i]);
	}
}	


void SpriteRenderer::clearVBO() {
	vbo.clear();
	count = 0;
}

void SpriteRenderer::pushToDevice() {
	vbo.pushToDevice(GL_STREAM_DRAW);
}

bool SpriteRenderer::init() {
	setVertexShader((char*)VERT_PATH);
	setGeometryShader((char*)GEOM_PATH);
	setFragmentShader((char*)FRAG_PATH);
	return buildProgram();
}

//TODO: just for debug
bool set = false;

void SpriteRenderer::draw(glm::vec2 screensize, Camera* camera) {
	//glUseProgram(shaderProgram); //moved to core
	bindProgram();
	glBindVertexArray(vao);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	//
	//if (!set) {
	//	set = true;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1.getID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex2.getID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tex3.getID());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tex4.getID());
	//}
	glUniform2fv(sprShaUni::screensize, 1, &screensize[0]);
	if (cammode == 0) {
		proj = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y, -10000.0f, 10000.0f);
	}
	else if (cammode == 1) {
		proj = glm::perspectiveFov(camera->getFoV(), screensize.x, screensize.y, 0.1f, 10000.0f);
	}
	model = glm::mat4(1, 0, 0, 0,
		              0,1,0,0,
		              0,0,1,0,
		              0,0,0,1);
	//model = glm::scale(glm::mat4(1), glm::vec3(1/screensize.x, 1/screensize.y, 1/1000));
	//model = glm::scale(glm::mat4(1), glm::vec3(100));
	view = camera->getView();
	//view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	mvp = proj * view * model;
	glUniformMatrix4fv(sprShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	// Draw:
	glDrawArrays(GL_POINTS, 0, count);
	// Clean up
	glBindVertexArray(0);
	glUseProgram(0);
	//unbind tex
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SpriteRenderer::deleteShaders() {
	deleteProgram();
}

SpriteRenderer::SpriteRenderer()
{
}


SpriteRenderer::~SpriteRenderer()
{
	deleteShaders();
}
