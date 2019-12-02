/**
 * Keeps track of a graphic on the screen.
 *
 * @date: 4/5/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "UISprite.h"

void UIShapes::createCircle(UIShapes::UICircle* c, glm::vec2 pos, float radius, float thickness, glm::vec4 color) {
	c->z = 0;
	c->position = pos;
	c->radius1 = radius;
	c->radius2 = thickness;
	c->highlight = glm::vec4(0);
	c->color = color;
}

void UIHandler::addShape(UIShapes::UICircle* shape) {
	circles.push_back(shape);
}

void UIHandler::addShape(UISprite* sprite) {
	sprites.push_back(sprite);
}


bool UIHandler::dragTest(bool drag, bool dragBegin, glm::vec2 mousePos, glm::vec2 dragDelta, float scrollDelta) {

	bool clearDrag = false;
	int topIdx = -1;
	int topZ = 0;

	// remove marked shapes
	int tgt = circles.size();
	for (int i = 0; i < tgt; ++i) {
		UIShapes::UICircle* c = circles[i];
		if (c->highlight.w != 0) {
			circles.erase(circles.begin() + i);
			tgt -= 1;
			i -= 1;
		}
	}

	bool circle = false;

	// find the shape on top (with max z)
	for (int i = 0; i < circles.size(); ++i) {
		UIShapes::UICircle* c = circles[i];
		glm::vec2 dst = ((mousePos - c->position)*(mousePos - c->position));
		float dstlen = dst.x + dst.y;
		c->highlight.x = (dstlen < c->radius1 * c->radius1)?1.0f:0.0f;
		if (c->highlight.x && c->z >= topZ) {
			topZ = c->z;
			topIdx = i;
			circle = true;
		}
	}

	// find the shape on top (with max z)
	if(!circle)
		for (int i = 0; i < sprites.size(); ++i) {
			UISprite* s = sprites[i];
			s->highlight = s->selectible && s->testOverlap(&mousePos) ? 1.0f : 0.0f;
			if (s->selectible && s->testOverlap(&mousePos) /*&& s->z >= topZ*/) {
				/*topZ = c->z;*/
				topIdx = i;
			}
		}

	// unmark all but the top element
	for (int i = 0; i < circles.size(); ++i) {
		UIShapes::UICircle* c = circles[i];
		if (i != topIdx) {
			c->highlight.x = false;
		}
	}
	// unmark all but the top element
	if (!circle)
	for (int i = 0; i < sprites.size(); ++i) {
		UISprite* s = sprites[i];
		if (i != topIdx) {
			s->highlight = false;
		}
	}

	//do drag check
	for (int i = 0; i < circles.size(); ++i) {
		UIShapes::UICircle* c = circles[i];
		if (dragBegin && c->highlight.x) {
			c->highlight.y = true;
		}
		if (!drag) {
			c->highlight.y = false;
		}
		if (c->highlight.y) {
			c->position += dragDelta;
			clearDrag = true;
		}
	}

	for (int i = 0; i < sprites.size(); ++i) {
		UISprite* s = sprites[i];
		if (!circle)
		if (dragBegin && s->testOverlap(&mousePos)) {
			s->highlight = true;
		}
		if (!drag) {
			s->highlight = false;
		}
		if (!circle)
		if (s->highlight) {
			s->position += dragDelta;
			clearDrag = true;
		}
	}

	//do drag check
	if (circle)
		if (topIdx >= 0) {
			circles[topIdx]->radius1 += 10 * scrollDelta;
			if (circles[topIdx]->radius1 < 1)
				circles[topIdx]->radius1 = 1;
		}
	if (!circle)
		if (topIdx >= 0) {
			sprites[topIdx]->size *= 1 + 0.1 * scrollDelta;
		}

	return clearDrag;
}

//

UISprite::UISprite(GLuint _texture, glm::vec2 _position, glm::vec2 _size) {
	texid = _texture;
	position = _position;
	size = _size;
	uvstart = glm::vec2(0, 0);
	uvsize = glm::vec2(1, 1);
	angle = 0;
	selectible = true;
	color = glm::vec4(1, 0, 1, 1);
}

bool UISprite::testOverlap(glm::vec2* point) {
	return (point->x > position.x - 0.5*size.x) && (point->x < position.x + 0.5*size.x) &&
		   (point->y > position.y - 0.5*size.y) && (point->y < position.y + 0.5*size.y);
}

UISprite::~UISprite() {

}