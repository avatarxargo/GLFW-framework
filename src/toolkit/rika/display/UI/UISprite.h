/**
 * Keeps track of a graphic on the screen.
 *
 * @date: 4/5/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef UI_SPRITE_H
#define UI_SPRITE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

namespace UIShapes {

	struct UIElement {
		std::vector<GLuint> elements;
	};

	struct UILine {
		int z;
		glm::vec4 from;
		glm::vec4 to;
		glm::vec4 highlight;
		glm::vec4 color;
	};

	struct UILineStrip {
		int z;
		bool loop;
		std::vector<UILine> lines;
	};

	struct UICircle {
		int z;
		glm::vec2 position;
		float radius1;
		float radius2;
		glm::vec4 highlight;
		glm::vec4 color;
	};

	void createCircle(UIShapes::UICircle* c, glm::vec2 pos, float radius, float thickness, glm::vec4 color);
}

class UISprite {
public:
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 uvstart;
	glm::vec2 uvsize;
	glm::vec4 color;
	GLuint texid;
	bool highlight = false;
	float angle;
	bool selectible = true;

	bool testOverlap(glm::vec2* point);

	UISprite(GLuint texture, glm::vec2 pos, glm::vec2 size);
	~UISprite();
};


class UIHandler {
public:
	int selected = -1;
	std::vector<UIShapes::UICircle*> circles;
	std::vector<UISprite*> sprites;
	void addShape(UIShapes::UICircle* shape);
	void addShape(UISprite* sprite);
	/**Returns true if the drag delta should be cleared.*/
	bool dragTest(bool drag, bool dragBegin, glm::vec2 mousePos, glm::vec2 dragDelta, float scrollDelta);
};
#endif
