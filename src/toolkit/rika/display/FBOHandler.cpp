/**
 * Maintains FBOs and multipas render target.
 *
 * @date: 4/24/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "FBOHandler.h"

bool FBOHandler::createVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Create pass 2 VBO
	vbo.create();
	vbo.insert(glm::vec4(-1, -1, 0, 1));
	vbo.insert(glm::vec4(0, 0, 0, 1));

	vbo.insert(glm::vec4(1, -1, 0, 1));
	vbo.insert(glm::vec4(1, 0, 0, 1));

	vbo.insert(glm::vec4(-1, 1, 0, 1));
	vbo.insert(glm::vec4(0, 1, 0, 1));

	vbo.insert(glm::vec4(1, 1, 0, 1));
	vbo.insert(glm::vec4(1, 1, 0, 1));
	vbo.pushToDevice(GL_STATIC_DRAW);

	int len = 8;
	glEnableVertexAttribArray(fboShaAtt::pos);
	glVertexAttribPointer(fboShaAtt::pos, 4, GL_FLOAT, false, len * sizeof(float), 0);
	glEnableVertexAttribArray(fboShaAtt::uv);
	glVertexAttribPointer(fboShaAtt::uv, 4, GL_FLOAT, false, len * sizeof(float), (void*)(4 * sizeof(float)));
	glBindVertexArray(0);

	return false;
}

bool FBOHandler::init() {

	//build program
	setVertexShader((char*)VERT_PATH);
	setFragmentShader((char*)FRAG_PATH);
	buildProgram();

	//Things for the second pass
	createVAO();

	// Create the screen texture
	glGenTextures(1, &fbotex1);
	glBindTexture(GL_TEXTURE_2D, fbotex1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo_texture_width, fbo_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Create the screen texture
	glGenTextures(1, &fbotex2);
	glBindTexture(GL_TEXTURE_2D, fbotex2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo_texture_width, fbo_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Create the screen texture
	glGenTextures(1, &fbotex3);
	glBindTexture(GL_TEXTURE_2D, fbotex3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo_texture_width, fbo_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create the framebuffer object
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//attach the texture we just created to color attachment 1
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbotex1, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fbotex2, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, fbotex3, 0);

	glGenRenderbuffers(1, &fborb);
	glBindRenderbuffer(GL_RENDERBUFFER, fborb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbo_texture_width, fbo_texture_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fborb);

	//unbind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	bindProgram();
	glm::mat4 mvp(1);
	glUniformMatrix4fv(universalShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	glUniform1i(fboShaUni::fbotex1, 0);
	glUniform1i(fboShaUni::fbotex2, 1);
	glUniform1i(fboShaUni::fbotex3, 2);

	return false;
}

void FBOHandler::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//Out variable in frag shader will be written to the texture attached to GL_COLOR_ATTACHMENT0.
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	//Make the viewport match the FBO texture size.
	glViewport(0, 0, fbo_texture_width, fbo_texture_height);
}

void FBOHandler::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, m_viewport, fbo_texture_height);
	//TODO 
}

void FBOHandler::clear() {
	std::vector<GLubyte> emptyData(fbo_texture_width * fbo_texture_height * 4, 0);
	glBindTexture(GL_TEXTURE_2D, fbotex1);
	glClearTexImage(fbotex1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &emptyData[0]);
	glBindTexture(GL_TEXTURE_2D, fbotex2);
	glClearTexImage(fbotex2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &emptyData[0]);
	glBindTexture(GL_TEXTURE_2D, fbotex3);
	glClearTexImage(fbotex3, 0, GL_RGBA, GL_UNSIGNED_BYTE, &emptyData[0]);
}

void FBOHandler::drawPass2(glm::vec2 screensize) {
	bindProgram();
	glBindVertexArray(vao);
	bindProgram();
	glUniform2fv(universalShaUni::screensize, 1, &screensize[0]);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH);
	//
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbotex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbotex2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, fbotex3);
	//
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//
	glBindVertexArray(0);
	glUseProgram(0);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH);
}

FBOHandler::FBOHandler() {

}

FBOHandler::~FBOHandler() {

}
