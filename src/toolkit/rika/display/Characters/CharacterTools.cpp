/**
 * Contains the structs and classes used to simulate characters with bones and skeletons
 *
 * @date: 4/8/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "CharacterTools.h"

namespace CharacterTools {

	BezierNode::BezierNode(glm::vec2 _pos, glm::vec2 _guideIn, glm::vec2 _guideOut) {
		pos = new UIShapes::UICircle();
		guideIn = new UIShapes::UICircle();
		guideOut = new UIShapes::UICircle();
		createCircle(pos, _pos, 20, 10, glm::vec4(1, 0.5, 0, 1));
		createCircle(guideIn, _pos+_guideIn, 10, 5, glm::vec4(1, 0.5, 0, 1));
		createCircle(guideOut, _pos+_guideOut, 10, 5, glm::vec4(1, 0.5, 0, 1));
	}

	void BezierShape::addNode(BezierNode* node) {
		nodes.push_back(node);
		uiHandler->addShape(node->pos);
		uiHandler->addShape(node->guideIn);
		uiHandler->addShape(node->guideOut);
	}

	BezierShape::BezierShape(UIHandler* _uiHandler) {
		uiHandler = _uiHandler;
	}

	Node::Node() {
		control.color = glm::vec4(0.5, 0, 0.5, 1);
		control.radius1 = 50;
		control.radius2 = 5;
	}

	Bone::Bone(Node* _parent, Node* _child) {
		parent = _parent;
		child = _child;
		color = glm::vec4(1, 0, 1, 1);
		highlight = glm::vec4(1);
		transform = glm::mat4(1.0);
		parent->children.push_back(this);
		child->parent = this;
	}

	Bone* Skeleton::addBone(Node* node) {
		Node* parent = node;
		if (node == 0) {
			//create new root - also add it to nodes
			root = new Node();
			parent = root;
			root->id = nodes.size();
			nodes.push_back(root);
			uiHandler->addShape(&root->control);
		}
		//creates new bone and a child the bone leads to.
		Node* child = new Node();
		Bone* bone = new Bone(parent, child);
		bones.push_back(bone);
		child->id = nodes.size();
		nodes.push_back(child);
		uiHandler->addShape(&child->control);
		return bone;
	}

	Skeleton::Skeleton(UIHandler* _uiHandler) {
		uiHandler = _uiHandler;
	}
}