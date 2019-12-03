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
#include "../toolkit/data/Mesh.h"
#include "../toolkit/data/Camera.h"

#include "../toolkit/rika/display/FBOHandler.h"

// =================== variables ==========================

static const std::string mesh_vertex_shader("shaders/gui_demo_vs.glsl");
static const std::string mesh_fragment_shader("shaders/gui_demo_fs.glsl");
static const std::string curve_vertex_shader("shaders/curve_vs.glsl");
static const std::string curve_geometry_shader("shaders/curve_gm.glsl");
static const std::string curve_fragment_shader("shaders/curve_fs.glsl");
static std::vector <std::string> mesh_names = { "data/doremy.obj", "data/tstglobe.obj" };//   ""data/akko.obj"","data/girl.obj",

GLuint mesh_shader_program = -1;
GLuint curve_shader_program = -1;
GLuint texture_id = -1;

// =================== shaders ==========================

FBOHandler fbohandler;

// =================== models ==========================

// V Here it is declared
toolkit::Camera cameraStats;
float ccol[4] = { 10.0f / 255.0f,19.0f / 255.0f,28.0f / 255.0f,1.0f };

struct Settings {
	bool paintSurfaces = true;
	bool paintLineart = true;
	bool paintTrMesh = false;
	bool clear = true;
	float threshold = 0.5;
	int outlineMode = 1;
} settings;

struct LightStats {
	float pos[3] = { 0.0f,0.0f,0.0f };
	float falloff[3] = { 1.0f, 0.0f,0.0f };
	float ambient[3] = { 0.0f,0.0f,0.0f };
	float diffuse[3] = { 0.0f,0.0f,0.0f };
	float specular[3] = { 0.0f,0.0f,0.0f };
};

struct OutlineEbo {
	GLuint numElements;
	std::vector<GLuint> eboData = {};
};

struct MeshStats {
	bool visible = true;
	std::string meshName = "NO NAME";
	float angle = 0.0f;
	float meshpos[3] = { 0.0f,0.0f,0.0f };
	float scale[3] = { 1.0f,1.0f,1.0f };
	float params[4] = { .0f,0.0f,0.0f,0.0f };
	//material properties
	float ambient[3] = { 0.0f,0.0f,0.0f };
	float diffuse[3] = { 0.0f,0.0f,0.0f };
	float specular[3] = { 0.0f,0.0f,0.0f };
	int shininess = 100;
	bool textured = true;
	//MeshData meshData;
	toolkit::TrMesh trMesh;
	//OutlineEbo outlines;
};

std::vector<MeshStats> meshes;
std::vector<LightStats> lights = {
	{{0,0.7,0}, {0.0f,1.0f,0.0f}, {0.1f,0.1f,0.1f}, {1.0f,1.0f,1.0f}, {1.0f,1.0f,1.0f}}
};

// ====================================================

// looks at the mesh from the current camera and detects outlines
OutlineEbo generateOutlines(MeshStats &mesh) {
	OutlineEbo ret;
	int size = mesh.trMesh.elementCount-1;
	for (int i = 0; i < size; ++i) {
		ret.eboData.push_back(i);
		ret.eboData.push_back(i+1);
	}
	ret.numElements = size*2;
	return ret;
}

void addMesh(std::string path) {
	//MeshData meshData = LoadMesh(path);
	MeshStats newMesh = {
		true,
		path,
		0.0f,
		{ 0.0f,0.0f,0.0f },
		{ 1.0f,1.0f,1.0f },
		{ .0f,0.0f,0.0f,0.0f },
		//material
		{ 1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f },
		20.0f,
		true,
		toolkit::loadTrMeshFromFile(path)
	};
	toolkit::pushTrMeshBuffers(newMesh.trMesh, mesh_shader_program);
	meshes.push_back(newMesh);
}

void reload_shader()
{
	GLuint new_shader = InitShader(mesh_vertex_shader.c_str(), mesh_fragment_shader.c_str());

	//mesh
	if (new_shader == -1) // loading failed
	{
		glClearColor(1.0f, 0.0f, 1.0f, 0.0f); //change clear color if shader can't be compiled
	}
	else
	{
		glClearColor(ccol[0], ccol[1], ccol[2], ccol[3]);

		if (mesh_shader_program != -1)
		{
			glDeleteProgram(mesh_shader_program);
		}
		mesh_shader_program = new_shader;
	}

	new_shader = InitShader(curve_vertex_shader.c_str(), curve_fragment_shader.c_str());
	//curve
	if (new_shader == -1) // loading failed
	{
		glClearColor(1.0f, 1.0f, 0.0f, 0.0f); //change clear color if shader can't be compiled
	}
	else
	{
		glClearColor(ccol[0], ccol[1], ccol[2], ccol[3]);

		if (curve_shader_program != -1)
		{
			glDeleteProgram(curve_shader_program);
		}
		curve_shader_program = new_shader;
	}
}

// =================== inputs =======================

bool ML = false;
bool MR = false;
bool MM = false;

bool cameraRotate = false;
bool cameraLift = false;
bool cameraZoom = false;
bool motionst = false;

double lx, ly;
bool mouseover_imgui = false;

// any mouse movement
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	int width, height;
	glfwGetWindowSize(toolkit::getWindow(),&width,&height);
	double dx = xpos - lx;
	double dy = ypos - ly;
	if (mouseover_imgui || motionst) {
		motionst = false;
		dx = 0;
		dy = 0;
	}
	if (ML|| MR || MM) {
		cameraStats.camangle -= 6*dx / width;
	}
	if (ML) {
		cameraStats.campos[1] += 3 * dy / height;
	}
	if (MM) {
		cameraStats.campitch += 5 * dy / height;
	}
	if (MR) {
		cameraStats.camdst -= 5 * dy / height;
	}
	lx = xpos;
	ly = ypos;
}

// pressing or lifting the mouse buttons
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		ML = action == GLFW_PRESS;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		MR = action == GLFW_PRESS;
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		MM = action == GLFW_PRESS;
	//
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//begin cmera rotation
		cameraRotate = true;
		cameraLift = true;
		cameraZoom = false;
		motionst = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//begin cmera rotation
		cameraRotate = true;
		cameraLift = true;
		cameraZoom = false;
		motionst = true;
	}
	if (action == GLFW_RELEASE) {
		//end cmera rotation
		cameraRotate = false;
		cameraLift = false;
		cameraZoom = false;
	}
}

// hovering, leaving or entering the window
void cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		// The cursor entered the content area of the window
	}
	else
	{
		// The cursor left the content area of the window
	}
}

// ==================================================

// taken from: https://github.com/ocornut/imgui/issues/707
void CherryTheme() {
	// cherry colors, 3 intensities
#define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
#define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
#define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
// backgrounds (@todo: complete with BG_MED, BG_LOW)
#define BG(v)   ImVec4(0.200f, 0.220f, 0.270f, v)
// text
#define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

	auto &style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = TEXT(0.78f);
	style.Colors[ImGuiCol_TextDisabled] = TEXT(0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = BG(0.58f);
	style.Colors[ImGuiCol_PopupBg] = BG(0.9f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = BG(1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = MED(1.00f);
	style.Colors[ImGuiCol_TitleBg] = LOW(1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = HI(1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = BG(0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = BG(1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = MED(1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
	style.Colors[ImGuiCol_ButtonActive] = MED(1.00f);
	style.Colors[ImGuiCol_Header] = MED(0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
	style.Colors[ImGuiCol_HeaderActive] = HI(1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = MED(1.00f);
	style.Colors[ImGuiCol_PlotLines] = TEXT(0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = MED(1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = TEXT(0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = MED(1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
	// [...]
	style.Colors[ImGuiCol_ModalWindowDarkening] = BG(0.73f);

	style.WindowPadding = ImVec2(6, 4);
	style.WindowRounding = 0.0f;
	style.FramePadding = ImVec2(5, 2);
	style.FrameRounding = 3.0f;
	style.ItemSpacing = ImVec2(7, 1);
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;

	style.WindowTitleAlign.x = 0.50f;

	style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
	style.FrameBorderSize = 0.0f;
	style.WindowBorderSize = 1.0f;
}

void registerCallbacks() {
	glfwSetCursorPosCallback(toolkit::getWindow(), cursor_position_callback);
	glfwSetMouseButtonCallback(toolkit::getWindow(), mouse_button_callback);
	glfwSetCursorEnterCallback(toolkit::getWindow(), cursor_enter_callback);
}

void createScene() {
	CherryTheme();
	reload_shader();
	for (auto mesh_name : mesh_names) {
		addMesh(mesh_name);
	}
	texture_id = LoadTexture("data/DoremyTexture.png");
	meshes[0].meshpos[0] = 0.11f;
	meshes[0].meshpos[1] = 0.85f;
	registerCallbacks();
	fbohandler.createVAO();
	glfwMaximizeWindow(toolkit::getWindow());
	const GLFWimage iconImage = LoadGLFWImage("data/images/icon32.png");
	glfwSetWindowIcon(toolkit::getWindow(), 1, &iconImage);
	glfwSetWindowTitle(toolkit::getWindow(), "Advanced Anime Shader");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
	glLineWidth(5);
}

void applyMaterialParams(GLuint shaderId, MeshStats &mesh) {
	glUseProgram(shaderId);
	int amb_loc = glGetUniformLocation(shaderId, "mamb");
	if (amb_loc != -1)
		glUniform3f(amb_loc, mesh.ambient[0], mesh.ambient[1], mesh.ambient[2]);
	int diff_loc = glGetUniformLocation(shaderId, "mdiff");
	if (diff_loc != -1)
		glUniform3f(diff_loc, mesh.diffuse[0], mesh.diffuse[1], mesh.diffuse[2]);
	int spec_loc = glGetUniformLocation(shaderId, "mspec");
	if (spec_loc != -1)
		glUniform3f(spec_loc, mesh.specular[0], mesh.specular[1], mesh.specular[2]);
	int shine_loc = glGetUniformLocation(shaderId, "mshininess");
	if (shine_loc != -1)
		glUniform1i(shine_loc, mesh.shininess);
	int textured_loc = glGetUniformLocation(shaderId, "mtextured");
	if (textured_loc != -1)
		glUniform1i(textured_loc, mesh.textured);
}

void applyLightParams(GLuint shaderId, LightStats &light) {
	glUseProgram(shaderId);
	int lpos_loc = glGetUniformLocation(shaderId, "lpos");
	if (lpos_loc != -1) {
		glm::vec4 lposraw = glm::vec4(light.pos[0], light.pos[1], light.pos[2], 1);
		glm::vec4 camLight = cameraStats.V * lposraw;
		glUniform3f(lpos_loc, camLight[0], camLight[1], camLight[2]);
	}
	int amb_loc = glGetUniformLocation(shaderId, "lamb");
	if (amb_loc != -1)
		glUniform3f(amb_loc, light.ambient[0], light.ambient[1], light.ambient[2]);
	int diff_loc = glGetUniformLocation(shaderId, "ldiff");
	if (diff_loc != -1)
		glUniform3f(diff_loc, light.diffuse[0], light.diffuse[1], light.diffuse[2]);
	int spec_loc = glGetUniformLocation(shaderId, "lspec");
	if (spec_loc != -1)
		glUniform3f(spec_loc, light.specular[0], light.specular[1], light.specular[2]);
	int falloff_loc = glGetUniformLocation(shaderId, "lfalloff");
	if (falloff_loc != -1)
		glUniform3f(falloff_loc, light.falloff[0], light.falloff[1], light.falloff[2]);
}

void drawMesh(MeshStats &mesh) {
	if (!mesh.visible)
		return;
	applyMaterialParams(mesh_shader_program, mesh);
	glm::mat4 T = glm::translate(glm::vec3(mesh.meshpos[0], mesh.meshpos[1], mesh.meshpos[2]));
	cameraStats.M = T * glm::rotate(mesh.angle*3.14159f/180.0f, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(mesh.scale[0] * mesh.trMesh.scale, mesh.scale[1] * mesh.trMesh.scale, mesh.scale[2] * mesh.trMesh.scale));

	glm::vec3 eye = glm::vec3(glm::cos(cameraStats.campitch)*glm::sin(cameraStats.camangle)*cameraStats.camdst, glm::sin(cameraStats.campitch)*cameraStats.camdst, glm::cos(cameraStats.campitch)*glm::cos(cameraStats.camangle)*cameraStats.camdst) + glm::vec3(cameraStats.campos[0], cameraStats.campos[1], cameraStats.campos[2]);
	glm::vec3 tgt = glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(cameraStats.campos[0], cameraStats.campos[1], cameraStats.campos[2]);
	cameraStats.V = glm::lookAt(eye, tgt, glm::vec3(0.0f, 1.0f, 0.0f));
	int width, height;
	glfwGetWindowSize(toolkit::getWindow(), &width, &height);
	cameraStats.P = glm::perspective((cameraStats.camstats[0] * 3.14159f / 180.0f), ((float)width) / height, cameraStats.camstats[1], cameraStats.camstats[2]);

	glUseProgram(mesh_shader_program);
	//glPolygonMode(GL_FRONT, GL_TRIANGLES);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	int PVM_loc = glGetUniformLocation(mesh_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		cameraStats.PVM = cameraStats.P * cameraStats.V * cameraStats.M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(cameraStats.PVM));
	}

	int P_loc = glGetUniformLocation(mesh_shader_program, "P");
	if (P_loc != -1)
	{
		glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(cameraStats.P));
	}

	int VM_loc = glGetUniformLocation(mesh_shader_program, "VM");
	if (VM_loc != -1)
	{
		glm::mat4 VM = cameraStats.V * cameraStats.M;
		glUniformMatrix4fv(VM_loc, 1, false, glm::value_ptr(VM));
	}

	int V_loc = glGetUniformLocation(mesh_shader_program, "myV");
	if (V_loc != -1)
	{
		//glm::mat4 myV = glm::inverseTranspose(V);
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(cameraStats.V));
	}

	int N_loc = glGetUniformLocation(mesh_shader_program, "N");
	if (N_loc != -1)
	{
		cameraStats.N = glm::inverseTranspose(cameraStats.V*cameraStats.M);
		/*std::cout << cameraStats.N[0][0] << "," << cameraStats.N[0][1] << "," << cameraStats.N[0][2] << "," << cameraStats.N[0][3] << "\n";
		std::cout << cameraStats.N[1][0] << "," << cameraStats.N[1][1] << "," << cameraStats.N[1][2] << "," << cameraStats.N[1][3] << "\n";
		std::cout << cameraStats.N[2][0] << "," << cameraStats.N[2][1] << "," << cameraStats.N[2][2] << "," << cameraStats.N[2][3] << "\n";
		std::cout << cameraStats.N[3][0] << "," << cameraStats.N[3][1] << "," << cameraStats.N[3][2] << "," << cameraStats.N[3][3] << "]\n";
		*/glUniformMatrix4fv(N_loc, 1, false, glm::value_ptr(cameraStats.N));
	}

	int tex_loc = glGetUniformLocation(mesh_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	//if (settings.paintTrMesh) {
		glBindVertexArray(mesh.trMesh.vao);
		glDrawElements(GL_TRIANGLES, mesh.trMesh.elementCount*3, GL_UNSIGNED_INT, 0);
	/*}
	else {
		glBindVertexArray(mesh.meshData.mVao);
		glDrawElements(GL_TRIANGLES, mesh.meshData.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	}*/
}


void drawMeshOutlines(MeshStats &mesh) {
	if (!mesh.visible)
		return;
	applyMaterialParams(curve_shader_program, mesh);
	glm::mat4 T = glm::translate(glm::vec3(mesh.meshpos[0], mesh.meshpos[1], mesh.meshpos[2]));
	glm::mat4 M = T * glm::rotate(mesh.angle*3.14159f / 180.0f, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(mesh.scale[0] * mesh.trMesh.scale, mesh.scale[1] * mesh.trMesh.scale, mesh.scale[2] * mesh.trMesh.scale));

	glm::vec3 eye = glm::vec3(glm::cos(cameraStats.campitch)*glm::sin(cameraStats.camangle)*cameraStats.camdst, glm::sin(cameraStats.campitch)*cameraStats.camdst, glm::cos(cameraStats.campitch)*glm::cos(cameraStats.camangle)*cameraStats.camdst) + glm::vec3(cameraStats.campos[0], cameraStats.campos[1], cameraStats.campos[2]);
	glm::vec3 tgt = glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(cameraStats.campos[0], cameraStats.campos[1], cameraStats.campos[2]);
	cameraStats.V = glm::lookAt(eye, tgt, glm::vec3(0.0f, 1.0f, 0.0f));
	int width, height;
	glfwGetWindowSize(toolkit::getWindow(), &width, &height);
	cameraStats.P = glm::perspective((cameraStats.camstats[0] * 3.14159f / 180.0f), ((float)width) / height, cameraStats.camstats[1], cameraStats.camstats[2]);

	glUseProgram(curve_shader_program);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	int PVM_loc = glGetUniformLocation(curve_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		cameraStats.PVM = cameraStats.P * cameraStats.V* M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(cameraStats.PVM));
	}

	int P_loc = glGetUniformLocation(curve_shader_program, "P");
	if (P_loc != -1)
	{
		glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(cameraStats.P));
	}

	int VM_loc = glGetUniformLocation(curve_shader_program, "VM");
	if (VM_loc != -1)
	{
		glm::mat4 VM = cameraStats.V * M;
		glUniformMatrix4fv(VM_loc, 1, false, glm::value_ptr(VM));
	}

	int V_loc = glGetUniformLocation(curve_shader_program, "myV");
	if (V_loc != -1)
	{
		//glm::mat4 myV = glm::inverseTranspose(V);
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(cameraStats.V));
	}

	int N_loc = glGetUniformLocation(curve_shader_program, "N");
	if (N_loc != -1)
	{
		cameraStats.N = glm::inverseTranspose(cameraStats.V*M);
		glUniformMatrix4fv(N_loc, 1, false, glm::value_ptr(cameraStats.N));
	}

	int tex_loc = glGetUniformLocation(mesh_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	glBindVertexArray(mesh.trMesh.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.trMesh.outlineEbo);
	glDrawElements(GL_LINES, mesh.trMesh.outline.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.trMesh.ebo);
	glBindVertexArray(0);
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

bool first = true;

//Draw the ImGui user interface
void draw_gui()
{
	ImGui::Begin("Camera Sliders"); {
		ImGui::Checkbox("Clear", &settings.clear);
		if (ImGui::ColorEdit4("Clear Color", ccol, true)) {
			glClearColor(ccol[0], ccol[1], ccol[2], ccol[3]);
		}
		ImGui::SliderFloat3("Camera Position", cameraStats.campos, -10.0f, +10.0f);
		ImGui::Columns(3);
		ImGui::SliderFloat("FoV", &(cameraStats.camstats[0]), 1, 179);
		ImGui::NextColumn();
		ImGui::SliderFloat("Near", &(cameraStats.camstats[1]), 0.01f, 100.0f);
		ImGui::NextColumn();
		ImGui::SliderFloat("Far", &(cameraStats.camstats[2]), 1.0f, 100.0f);
		ImGui::Columns(1);
		ImGui::SliderFloat("Camera Angle", &cameraStats.camangle, -3.14159f, +3.14159f);
		ImGui::SliderFloat("Camera Pitch", &cameraStats.campitch, -3.14159f, +3.14159f);
		ImGui::SliderFloat("Camera Distance", &cameraStats.camdst, 1, 20);
		if (ImGui::Button("Reload Shaders")) {
			reload_shader();
		}
	}
	ImGui::End();

	// Meshes
	ImGui::Begin("Scene Lighting Sliders");
	{
		ImGui::Text("Shaders");
		ImGui::Indent();
		ImGui::Checkbox("Render Surfaces", &settings.paintSurfaces);
		ImGui::SameLine();
		ImGui::Checkbox("Render Lineart", &settings.paintLineart);
		ImGui::SliderAngle("Threshold:", &settings.threshold, 0, 360);
		if (ImGui::RadioButton("Outline: Face Angle", settings.outlineMode==0))
			settings.outlineMode = 0;
		ImGui::SameLine();
		if (ImGui::RadioButton("Outline: Camera Angle", settings.outlineMode == 1))
			settings.outlineMode = 1;
		ImGui::Unindent();
		//
		ImGui::Text("Lights");
		ImGui::Indent();
		for (int id = 0; id < lights.size(); ++id) {
			LightStats* current = &lights[id];
			if (ImGui::TreeNode("Point Light"))
			{
				ImGui::SliderFloat3("Position", current->pos, -3.0f, +3.0f);
				ImGui::SliderFloat3("Falloff(1,d,d^2)", current->falloff, 0, +3.0f);
				ImGui::ColorEdit3("Ambient", current->ambient);
				ImGui::ColorEdit3("Diffuse", current->diffuse);
				ImGui::ColorEdit3("Specular", current->specular);
				ImGui::TreePop();
			}
		}
		ImGui::Unindent();
		ImGui::Text("Meshes");
		ImGui::Indent();
		for (int id = 0; id < meshes.size(); ++id) {
			MeshStats* current = &meshes[id];
			if(first)
				ImGui::SetNextItemOpen(true);
			first = false;
			if (ImGui::TreeNode(current->meshName.c_str()))
			{
				ImGui::Indent();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "Placement");
				ImGui::Checkbox("Visible", &(current->visible));
				ImGui::SliderFloat("Angle", &(current->angle), -360.0f, +360.0f);
				ImGui::SliderFloat3("Position", current->meshpos, -3.0f, +3.0f);
				ImGui::SliderFloat3("Scale", current->scale, -3.0f, +3.0f);
				ImGui::SliderFloat4("Params", current->params, 0.0f, 1.0f);
				//material
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "Material");
				ImGui::ColorEdit3("Ambient", current->ambient);
				ImGui::ColorEdit3("Diffuse", current->diffuse);
				ImGui::ColorEdit3("Specular", current->specular);
				ImGui::SliderInt("Shininess", &(current->shininess), 1, 100);
				ImGui::Checkbox("Textured", &(current->textured));
				ImGui::Unindent();
				ImGui::TreePop();
			}
		}
		ImGui::Unindent();
	}
	ImGui::End();
	mouseover_imgui = false;
	mouseover_imgui |= ImGui::IsAnyWindowHovered();
	mouseover_imgui |= ImGui::IsAnyItemHovered();
}

void renderScene() {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if(settings.clear)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (lights.size() > 0) {
		applyLightParams(mesh_shader_program, lights[0]);
		applyLightParams(curve_shader_program, lights[0]);
	}
	if(settings.paintSurfaces)
		for (auto mesh : meshes) {
			drawMesh(mesh);
		}
	if (settings.paintLineart)
		for (auto mesh : meshes) {
			toolkit::genTrMeshGenLineart(mesh.trMesh, cameraStats, settings.threshold, settings.outlineMode);
			drawMeshOutlines(mesh);
		}

	draw_gui();

	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}