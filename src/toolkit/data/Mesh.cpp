///=========================================
///				Mesh.h
///	@author:	David Hrusa
///	@email:		hrusa@pansyn.com
///	@date:		11/21/2019
/// @desc:		Represents drawable mesh + loading methods from assimp
///
///=========================================

#include "Mesh.h"
#include <iostream>
#include <map>
#include <tuple>

// Create an instance of the Importer class
extern Assimp::Importer gImporter;

toolkit::TrMesh toolkit::loadTrMeshFromFile(const std::string& path) {
	TrMesh mesh;

	//check if file exists
	std::ifstream fin(path.c_str());
	if (!fin.fail())
	{
		fin.close();
	}
	else
	{
		printf("Couldn't open file: %s\n", path.c_str());
		printf("%s\n", gImporter.GetErrorString());
		return mesh;
	}

	const aiScene* scene = gImporter.ReadFile(path, aiProcessPreset_TargetRealtime_Quality | aiProcess_PreTransformVertices); //PreTransformVertices makes multiple submeshes work
																														 // If the import failed, report it
	if (!scene)
	{
		printf("%s\n", gImporter.GetErrorString());
		return mesh;
	}

	//determine scale
	aiVector3D vmin, vmax;
	GetBoundingBox(scene, &vmin, &vmax);
	aiVector3D diff = vmax - vmin;
	float w = std::max(diff.x, std::max(diff.y, diff.z));
	mesh.scale = 1.0f / w;

	// Now we can access the file's contents.
	printf("Import of scene %s succeeded.\n", path.c_str());
	// Process the mesh
	parseTrMesh(mesh, scene);
	return mesh;
}

int initial = 0;

void toolkit::genTrMeshGenLineart(toolkit::ComputeShader* shader, toolkit::TrMesh &trmesh, toolkit::Camera &camera, float threshold, int outlineMode) {
	//clear old data:
	trmesh.outline.clear();
	//execute shader:
	glUseProgram(shader->programID);
	shader->x = trmesh.face_info.size();
	// pass data:
	size_t buffersize = sizeof(TrFaceInfo)*trmesh.face_info.size();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, shader->inBufferA);
	glBufferData(GL_SHADER_STORAGE_BUFFER, buffersize, &trmesh.face_info[0], GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shader->inBufferA);
	if(initial == 10)
	for (int f = 0; f < trmesh.face_info.size() && f < 5; ++f) {
		std::cout << f << ":pre rating:" << trmesh.face_info[f].rating << "\n";
		std::cout << f << ":pre normal:" << trmesh.face_info[f].normal.x << "," << trmesh.face_info[f].normal.y << "," << trmesh.face_info[f].normal.z << "\n";
		std::cout << f << ":pre cast:" << trmesh.face_info[f].castnormal.x << "," << trmesh.face_info[f].castnormal.y << "," << trmesh.face_info[f].castnormal.z << "\n";
	}
	//uniforms
	
	int count_loc = glGetUniformLocation(shader->programID, "count");
	if (count_loc != -1)
		glUniform1i(count_loc, trmesh.face_info.size());

	int gcount_loc = glGetUniformLocation(shader->programID, "groupcount");
	if (gcount_loc != -1)
		glUniform2f(gcount_loc, 16, 16);

	int N_loc = glGetUniformLocation(shader->programID, "N");
	if (N_loc != -1)
	{
		camera.N = glm::inverseTranspose(camera.V*camera.M);
		glUniformMatrix4fv(N_loc, 1, false, glm::value_ptr(camera.N[0]));
	}

	toolkit::dispatchComputeShader(shader);
	// read it:
	//toolkit::TrFaceInfo meshinforead[6];
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	//memcpy(&meshinforead[0], p, sizeof(TrFaceInfo)*5);
	memcpy(trmesh.face_info.data(), p, buffersize);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);

	if (initial++ >= 10 && false) {
		std::cout << "\n";
		for (int f = 0; f < trmesh.face_info.size() && f < 5; ++f) {
			std::cout << f << ":post rating:" << trmesh.face_info[f].rating << "\n";
			std::cout << f << ":post normal:" << trmesh.face_info[f].normal.x << "," << trmesh.face_info[f].normal.y << "," << trmesh.face_info[f].normal.z << "\n";
			std::cout << f << ":post cast:" << trmesh.face_info[f].castnormal.x << "," << trmesh.face_info[f].castnormal.y << "," << trmesh.face_info[f].castnormal.z << "\n";
		}
	}
	//evaluate the faces (LEGACY CPU):
	/*for (int f = 0; f < trmesh.face_info.size(); ++f) {
		TrFaceInfo *face = &trmesh.face_info[f];
		//glm::vec4 projn = camera.N*glm::vec4(face->normal, 0);
		//face->rating = projn.z;
		glm::vec3 cam = { 0,0,0 };

		glm::vec3 pA = trmesh.verteces[trmesh.faces[f].a].pos;
		glm::vec3 pB = trmesh.verteces[trmesh.faces[f].b].pos;
		glm::vec3 pC = trmesh.verteces[trmesh.faces[f].c].pos;
		glm::vec4 ppA = camera.P*camera.V* camera.M * glm::vec4(pA, 1);
		glm::vec4 ppB = camera.P*camera.V*camera.M * glm::vec4(pB, 1);
		glm::vec4 ppC = camera.P*camera.V*camera.M * glm::vec4(pC, 1);

		glm::vec4 v0 = ppA - ppC;
		glm::vec4 v1 = ppB - ppC;
		//face->castnormal = glm::normalize(glm::cross(glm::vec3(v0.x, v0.y, v0.z), glm::vec3(v1.x, v1.y, v1.z)));
		
		if (initial++ >= 10 && f < 2)
		std::cout << f << ": GPU cast:" << trmesh.face_info[f].castnormal.x << "," << trmesh.face_info[f].castnormal.y << "," << trmesh.face_info[f].castnormal.z << "\n";

		glm::vec4 projn = glm::normalize(camera.N*face->normal);
		face->castnormal = projn;
		if (initial++ >= 10 && f < 2)
		std::cout << f <<  ": CPU cast:" << trmesh.face_info[f].castnormal.x << "," << trmesh.face_info[f].castnormal.y << "," << trmesh.face_info[f].castnormal.z << "\n";

		face->rating = face->castnormal.z;
	}*/
	/*face->rating = 0;
	face->rating += (ppA.x * ppB.y - ppB.x * ppA.y);
	face->rating += (ppB.x * ppC.y - ppC.x * ppB.y);
	face->rating += (ppC.x * ppA.y - ppA.x * ppC.y);*/

	for (int i = 0; i < trmesh.edges.size(); ++i) {
		TrEdge *edge = &trmesh.edges[i];
		bool highlight = false;
		//if (!edge->manifold)
		//	highlight = true;
		// evaluate:
		if (edge->faces.size() > 1) {
			TrFaceInfo* faceinfoA = &trmesh.face_info[edge->faces[0]];
			TrFaceInfo* faceinfoB = &trmesh.face_info[edge->faces[1]];
			if (outlineMode == 0) {
				float d = dot(faceinfoA->normal, faceinfoB->normal);
				if (d < threshold)
					highlight = true;
			}
			else if (outlineMode == 1) {
				highlight = (faceinfoA->rating>0 && faceinfoB->rating <=0) || (faceinfoA->rating <= 0 && faceinfoB->rating>0);
				/*if (faceinfoA->rating > -threshold && faceinfoB->rating < threshold)
					highlight = true;
				if (faceinfoB->rating > -threshold && faceinfoA->rating < threshold)
					highlight = true;*/

				//some weird, kinda working implementation:
				/*glm::vec4 nAv = glm::normalize(camera.N*glm::vec4(faceinfoA->normal, 0));
				glm::vec4 nBv = glm::normalize(camera.N*glm::vec4(faceinfoB->normal, 0));
				float d = glm::min(dot(nAv, glm::vec4(0,0,1,0)), dot(nBv, glm::vec4(0, 0, 1, 0)));
				if (d < threshold)
					highlight = true;*/

			}
		} else if (edge->faces.size() == 1) {
			if (outlineMode == 1) {
				TrFaceInfo* faceinfoA = &trmesh.face_info[edge->faces[0]];
				if (glm::abs(glm::dot(faceinfoA->castnormal, glm::vec4(0, 0, 1, 0))) < 0.1) {
					highlight = true;
					//std::cout << faceinfoA->castnormal.x << "," << faceinfoA->castnormal.y << "," << faceinfoA->castnormal.z << "\n";
				}
				highlight = false;
			}
		}
		// add:
		if (highlight) {
			trmesh.outline.push_back(trmesh.edges[i].from);
			trmesh.outline.push_back(trmesh.edges[i].to);
		}
	}
	pushTrOutlineBuffer(trmesh);
}

void toolkit::parseTrMesh(toolkit::TrMesh &trmesh, const aiScene* aiscene) {

	const int numSubmeshes = aiscene->mNumMeshes;
	{
		for (int m = 0; m < numSubmeshes; m++)
		{
			aiMesh* mesh = aiscene->mMeshes[m];
			if (mesh->HasPositions())
			{
				const int size = 3 * sizeof(float)*mesh->mNumVertices;
				const void* data = mesh->mVertices;

				//verteces
				for (int n = 0; n < (int)mesh->mNumVertices; n++)
				{
					TrVert vert;
					vert.pos = { mesh->mVertices[n].x ,mesh->mVertices[n].y, mesh->mVertices[n].z };
					if (mesh->HasNormals())
						vert.normal = { mesh->mNormals[n].x ,mesh->mNormals[n].y, mesh->mNormals[n].z };
					else {
						vert.normal = { 0,1,0 };
					}
					if(mesh->HasVertexColors(0))
						vert.color = { mesh->mColors[0][n].r ,mesh->mColors[0][n].g, mesh->mColors[0][n].b, mesh->mColors[0][n].a };
					else {
						vert.color = { 0,0,0,1 };
					}
					if (mesh->HasTextureCoords(0))
						vert.texcoord = { mesh->mTextureCoords[0][n].x, mesh->mTextureCoords[0][n].y };
					else {
						vert.texcoord = { 0,0 };
					}
					trmesh.verteces.push_back(vert);

					/*std::cout << "v: " << vert.pos.x << "," << vert.pos.y << "," << vert.pos.z << "\n";
					std::cout << "n: " << vert.normal.x << "," << vert.normal.y << "," << vert.normal.z << "\n";
					std::cout << "c: " << vert.color.x << "," << vert.color.y << "," << vert.color.z << "," << vert.color.a << "\n";
					std::cout << "t: " << vert.texcoord.x << "," << vert.texcoord.y << "\n";*/
				}
				//indeces
				for (int n = 0; n < (int)mesh->mNumFaces; n++)
				{
					TrFace face;
					if (mesh->mFaces[n].mNumIndices == 3) {
						face.a = mesh->mFaces[n].mIndices[0];
						face.b = mesh->mFaces[n].mIndices[1];
						face.c = mesh->mFaces[n].mIndices[2];
					}
					trmesh.faces.push_back(face);
					trmesh.face_info.push_back({});
				}
			}
		}
	}

	trmesh.elementCount = trmesh.faces.size();
	trmesh.vertCount = trmesh.verteces.size();
	generateTrMeshNormals(trmesh);
}

std::tuple<GLuint, GLuint> mingl(GLuint a, GLuint b) {
	if (a < b)
		return { a,b };
	else
		return { b,a };
}

void  toolkit::generateTrMeshNormals(toolkit::TrMesh &trmesh) {
	std::map<std::tuple<GLuint, GLuint>, int> edgemap;

	int edgeidx = 0;

	for (int f = 0; f < trmesh.elementCount; f++)
	{
		TrFace *face = &trmesh.faces[f];
		TrFaceInfo *faceinfo = &trmesh.face_info[f];

		//std::cout << "accesssing verts: " << face->a << "," << face->b << "," << face->c << " - max:" << trmesh.vertCount << "\n";
		// calculate the normal for each face:
		glm::vec3 v0 = trmesh.verteces[face->a].pos - trmesh.verteces[face->c].pos;
		glm::vec3 v1 = trmesh.verteces[face->b].pos - trmesh.verteces[face->c].pos;
		faceinfo->normal = glm::vec4(glm::normalize(glm::cross(v0, v1)),0);

		// in order to avoid duplicates we always order the edge ids lower to higher
		// generate unique lines as we traverse the faces:
		std::tuple<GLuint, GLuint> facepairs[3] = { mingl(face->a, face->b),
													mingl(face->b, face->c),
													mingl(face->c, face->a), };
		for (int i = 0; i < 3; ++i) {
			std::tuple<GLuint, GLuint> orderedpair = facepairs[i];
			auto it = edgemap.find(orderedpair);
			if (it != edgemap.end()) {
				//already exists - update edges:
				int lineidx = (*it).second;
				TrEdge* line = &trmesh.edges[lineidx];
				line->faces.push_back(f);
				line->manifold = line->faces.size() == 2;
			} else {
				//doesn't exist
				TrEdge line;
				line.idx = edgeidx++;
				line.from = std::get<0>(orderedpair);
				line.to = std::get<1>(orderedpair);
				line.faces.push_back(f);
				line.manifold = false;
				// If insertion fails, the element already exists:
				edgemap.insert(std::make_pair(orderedpair, line.idx));
				trmesh.edges.push_back(line);
			}
		}
	}

	/*for (int l = 0; l < (int)trmesh.edges.size(); l++) {
		std::cout << "l: " << trmesh.edges[l].manifold << "\n";
	}*/
}

void toolkit::pushTrMeshBuffers(TrMesh &mesh, GLint program) {

	if (mesh.vao != -1)
	{
		glDeleteVertexArrays(1, &mesh.vao);
	}

	if (mesh.ebo != -1)
	{
		glDeleteBuffers(1, &mesh.ebo);
	}

	if (mesh.vbo != -1)
	{
		glDeleteBuffers(1, &mesh.vbo);
	}

	if (program == -1)
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	//shader attrib locations
	const int pos_loc = 0;
	const int normal_loc = 1;
	const int tex_coord_loc = 2;
	const int color_loc = 3;

	glBindAttribLocation(program, pos_loc, "pos_attrib");
	glBindAttribLocation(program, normal_loc, "normal_attrib");
	glBindAttribLocation(program, tex_coord_loc, "tex_coord_attrib");
	glBindAttribLocation(program, color_loc, "color_attrib");

	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	/*Elements*/ {
		mesh.elementCount = mesh.faces.size();
		glGenBuffers(1, &mesh.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TrFace) * mesh.elementCount, &mesh.faces[0], GL_STATIC_DRAW);
	}

	/*verts*/ {
		mesh.vertCount = mesh.verteces.size();
		glGenBuffers(1, &mesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TrVert) * mesh.vertCount, &mesh.verteces[0], GL_STATIC_DRAW);
		// I made them interleaved because we are working with them in a struct
		GLsizei stride = sizeof(TrVert);
		glEnableVertexAttribArray(pos_loc);
		glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(TrVert, pos));
		glEnableVertexAttribArray(normal_loc);
		glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(TrVert, normal));
		glEnableVertexAttribArray(tex_coord_loc);
		glVertexAttribPointer(tex_coord_loc, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(TrVert, texcoord));
		glEnableVertexAttribArray(color_loc);
		glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(TrVert, color));
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	/*std::cout << "pos_loc: " << (GLvoid*)offsetof(TrVert, pos) << "\n";
	std::cout << "normal_loc: " << (GLvoid*)offsetof(TrVert, normal) << "\n";
	std::cout << "tex_coord_loc: " << (GLvoid*)offsetof(TrVert, texcoord) << "\n";
	std::cout << "color_loc: " << (GLvoid*)offsetof(TrVert, color) << "\n";
	std::cout << "verts: " << mesh.vertCount << "\n";
	std::cout << "eles: " << mesh.elementCount << "\n";*/
}


void toolkit::pushTrOutlineBuffer(TrMesh &mesh) {

	if (mesh.outlineEbo != -1)
	{
		glDeleteBuffers(1, &mesh.outlineEbo);
	}

	/*Elements*/ {
		glGenBuffers(1, &mesh.outlineEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.outlineEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.outline.size(), &mesh.outline[0], GL_DYNAMIC_DRAW);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

toolkit::Mesh::Mesh() {

};

toolkit::Mesh::~Mesh() {

};

void toolkit::Mesh::loadObj(const char* path) {

};



void toolkit::GetBoundingBox(const aiMesh* mesh, aiVector3D* min, aiVector3D* max)
{
	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;

	for (unsigned int t = 0; t < mesh->mNumVertices; ++t)
	{
		aiVector3D tmp = mesh->mVertices[t];

		min->x = std::min(min->x, tmp.x);
		min->y = std::min(min->y, tmp.y);
		min->z = std::min(min->z, tmp.z);

		max->x = std::max(max->x, tmp.x);
		max->y = std::max(max->y, tmp.y);
		max->z = std::max(max->z, tmp.z);
	}
}


void toolkit::GetBoundingBoxForNode(const aiScene* scene, const aiNode* nd, aiVector3D* min, aiVector3D* max)
{
	unsigned int n = 0, t;

	for (; n < nd->mNumMeshes; ++n)
	{
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t)
		{

			aiVector3D tmp = mesh->mVertices[t];

			min->x = std::min(min->x, tmp.x);
			min->y = std::min(min->y, tmp.y);
			min->z = std::min(min->z, tmp.z);

			max->x = std::max(max->x, tmp.x);
			max->y = std::max(max->y, tmp.y);
			max->z = std::max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n)
	{
		GetBoundingBoxForNode(scene, nd->mChildren[n], min, max);
	}
}


void toolkit::GetBoundingBox(const aiScene* scene, aiVector3D* min, aiVector3D* max)
{
	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	toolkit::GetBoundingBoxForNode(scene, scene->mRootNode, min, max);
}
