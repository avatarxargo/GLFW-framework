///=========================================
///				Mesh.h
///	@author:	David Hrusa
///	@email:		hrusa@pansyn.com
///	@date:		11/21/2019
/// @desc:		Represents drawable mesh + loading methods from assimp
///
///=========================================

#ifndef __MESH_H__
#define __MESH_H__

#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "assimp/Scene.h"
#include "assimp/Importer.hpp"
#include "assimp/PostProcess.h"

#include "Camera.h"

namespace toolkit {

	struct TrVert {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texcoord;
		glm::vec4 color;
	};

	//Traversible Mesh Representation
	struct TrFace {
		//verts by index:
		GLuint a, b, c;
	};
	struct TrFaceInfo {
		bool rating; //whether the face faces camera, for toon shading
		glm::vec3 normal;
	};

	struct TrEdge {
		int idx;
		GLuint from;
		GLuint to;
		bool manifold; //true if only 2 faces share this edge
		std::vector<int> faces;
	};

	struct TrMesh {
		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;
		unsigned int outlineEbo;
		int vertCount;
		int elementCount;
		float scale = 1.0;
		std::vector<TrVert> verteces;
		std::vector<TrFace> faces;
		std::vector<TrFaceInfo> face_info;
		std::vector<TrEdge> edges;
		std::vector<GLuint> outline;
	};

	TrMesh loadTrMeshFromFile(const std::string& pFile);
	void genTrMeshGenLineart(toolkit::TrMesh &trmesh, toolkit::Camera &camera, float threshold);
	void parseTrMesh(TrMesh &mesh, const  aiScene* aiscene);
	void generateTrMeshNormals(TrMesh &mesh);
	void pushTrMeshBuffers(TrMesh &mesh, GLint program);
	void pushTrOutlineBuffer(TrMesh &mesh);

	void GetBoundingBox(const aiMesh* mesh, aiVector3D* min, aiVector3D* max);
	void GetBoundingBoxForNode(const aiScene* scene, const aiNode* nd, aiVector3D* min, aiVector3D* max);
	void GetBoundingBox(const aiScene* scene, aiVector3D* min, aiVector3D* max);

	class Mesh {
	private:

	public:
		void loadObj(const char* path);
		Mesh();
		~Mesh();
	};

}

#endif