#include "scene.h"

#include <windows.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp> //This header defines basic glm types (vec3, mat4, etc)

//These headers define matrix transformations
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <glm/gtc/type_ptr.hpp> //This header defines helper functions (e.g. value_ptr(...)) for passing vectors and matrices to shaders

#include "../toolkit/toolkit.h"
#include "../toolkit/stolen/InitShader.h"
#include "../toolkit/stolen/LoadMesh.h"
#include "../toolkit/stolen/LoadTexture.h"

// =================== variables ==========================

static const std::string mesh_vertex_shader("shaders/gui_demo_vs.glsl");
static const std::string mesh_fragment_shader("shaders/gui_demo_fs.glsl");
static const std::string mesh_name = "data/akko.obj";

GLuint mesh_shader_program = -1;
GLuint texture_id = -1;

MeshData akko;

// V Here it is declared
float campos[3] = { 0.0f,0.0f,0.0f };
float camstats[3] = { 40.0f,1.0f,100.0f };
float camangle = 0.0f;
float camdst = 5.0f;
bool clear = true;

// ====================================================

void reload_shader()
{
	GLuint new_shader = InitShader(mesh_vertex_shader.c_str(), mesh_fragment_shader.c_str());

	//fish
	if (new_shader == -1) // loading failed
	{
		glClearColor(1.0f, 0.0f, 1.0f, 0.0f); //change clear color if shader can't be compiled
	}
	else
	{
		glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

		if (mesh_shader_program != -1)
		{
			glDeleteProgram(mesh_shader_program);
		}
		mesh_shader_program = new_shader;
	}
}

void createScene() {
	akko = LoadMesh(mesh_name);
	reload_shader();
	glEnable(GL_DEPTH_TEST);
}

void drawAkko() {
	glm::mat4 T = glm::translate(glm::vec3(0,0,0));
	float scl = 0.7;
	glm::mat4 M = T * glm::rotate(0.0f, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(scl, scl, scl));

	glm::vec3 eye = glm::vec3(glm::sin(camangle)*camdst, 1.0f, glm::cos(camangle)*camdst) + glm::vec3(campos[0], campos[1], campos[2]);
	glm::vec3 tgt = glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(campos[0], campos[1], campos[2]);
	glm::mat4 V = glm::lookAt(eye, tgt, glm::vec3(0.0f, 1.0f, 0.0f));
	int width, height;
	glfwGetWindowSize(toolkit::getWindow(), &width, &height);
	glm::mat4 P = glm::perspective(round(camstats[0]), ((float)width)/height, camstats[1], camstats[2]);

	glUseProgram(mesh_shader_program);

	int PVM_loc = glGetUniformLocation(mesh_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V*M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int P_loc = glGetUniformLocation(mesh_shader_program, "P");
	if (P_loc != -1)
	{
		glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(P));
	}

	int VM_loc = glGetUniformLocation(mesh_shader_program, "VM");
	if (VM_loc != -1)
	{
		glm::mat4 VM = V * M;
		glUniformMatrix4fv(VM_loc, 1, false, glm::value_ptr(VM));
	}

	int V_loc = glGetUniformLocation(mesh_shader_program, "myV");
	if (V_loc != -1)
	{
		glm::mat4 myV = glm::inverseTranspose(V);
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	int N_loc = glGetUniformLocation(mesh_shader_program, "N");
	if (N_loc != -1)
	{
		glm::mat3 N = glm::inverseTranspose(glm::mat3(V*M));
		glUniformMatrix3fv(N_loc, 1, false, glm::value_ptr(N));
	}

	/*int tex_loc = glGetUniformLocation(mesh_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}*/

	glBindVertexArray(akko.mVao);
	glDrawElements(GL_TRIANGLES, akko.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);

}

void updateScene(double delta) {

	const int time_ms = glfwGetTime();
	float time_sec = 0.001f*time_ms;

	//Pass time_sec value to the shaders
	int time_loc = glGetUniformLocation(mesh_shader_program, "time");
	if (time_loc != -1)
	{
		glUseProgram(mesh_shader_program);
		glUniform1f(time_loc, time_sec);
	}
}

void renderScene() {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glEnd();*/

	drawAkko();

	ImGui::Begin("Tst Window");
	ImGui::Text("Hello");
	if (ImGui::Button("Load Model")) {
		std::cout << "hi\n";
	}
	ImGui::End();

	ImGui::Begin("Camera Sliders");
	ImGui::Checkbox("Clear", &clear);
	ImGui::SliderFloat3("Camera Position", campos, -10.0f, +10.0f);
	ImGui::Columns(3);
	ImGui::SliderFloat("FoV", &(camstats[0]), 30, 170);
	ImGui::NextColumn();
	ImGui::SliderFloat("Near", &(camstats[1]), 1.0f, 100.0f);
	ImGui::NextColumn();
	ImGui::SliderFloat("Far", &(camstats[2]), 1.0f, 100.0f);
	ImGui::Columns(1);
	ImGui::SliderFloat("Camera Distance", &(camdst), 1.0f, 30.0f);
	ImGui::SliderFloat("Camera Angle", &camangle, -3.14159f, +3.14159f);
	ImGui::End();


	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}