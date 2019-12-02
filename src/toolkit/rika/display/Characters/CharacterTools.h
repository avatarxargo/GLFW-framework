/**
 * Contains the structs and classes used to simulate characters with bones and skeletons
 *
 * @date: 4/8/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef CHARACTER_TOOLS_H
#define CHARACTER_TOOLS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "../UI/UISprite.h"

using namespace std;


namespace CharacterTools {
	class BezierNode;
	class BezierShape;
	class Node;
	class Bone;
	class Skeleton;

	enum BoneType {
		CIRCLE, BEAN, CLOTH, IMG
	};

	class BezierNode {
	public:
		UIShapes::UICircle* pos;
		UIShapes::UICircle* guideIn;
		UIShapes::UICircle* guideOut;
		glm::vec4 highlight;
		glm::vec4 color;
		BezierNode() {}
		BezierNode(glm::vec2 _pos, glm::vec2 _guideIn, glm::vec2 _guideOut);
	};

	//Always loops back on itself so the last nodes out will blend towards first nodes in.
	class BezierShape {
	public:
		vector<BezierNode*> nodes;
		UIHandler* uiHandler;
		//
		BezierShape() {}
		BezierShape(UIHandler* _uiHandler);
		void addNode(BezierNode* node);
	};

	struct BoneGraphic {
		glm::mat4 transform;
		glm::vec3 color;
	};

	class Node {
	public:
		//connections
		int id;
		Bone* parent;
		vector<Bone*> children;
		//
		UIShapes::UICircle control;
		Node();
	};

	class Bone {
	public:
		//connections
		Node* parent;
		Node* child;
		BoneType type = BEAN;
		glm::vec4 highlight = glm::vec4(0);
		glm::vec4 color = glm::vec4(1,0,1,1);
		glm::mat4 transform = glm::mat4(1.0);
		bool root = false;
		Bone() {}
		Bone(Node* parent, Node* child);
	};

	class Skeleton {
	public:
		UIHandler* uiHandler;
		string name;
		vector<Bone*> bones;
		vector<Node*> nodes;
		Node* root;
		//
		Bone* addBone(Node* node);
		void RemoveBone(int boneIdx);
		void RemoveBoneCascade(Node* node);
		Skeleton() {}
		Skeleton(UIHandler* _uiHandler);
	};

}

#endif