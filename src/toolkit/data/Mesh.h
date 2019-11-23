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

#include "assimp/Scene.h"
#include "assimp/Importer.hpp"
#include "assimp/PostProcess.h"

namespace toolkit {

	class Mesh {
	private:

	public:
		void loadObj(const char* path);
		Mesh();
		~Mesh();
	};

}

#endif