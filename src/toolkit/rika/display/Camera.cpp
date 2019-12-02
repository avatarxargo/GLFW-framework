/**
 * Keeps track of camera variables.
 *
 * @date: 4/18/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "Camera.h"

Camera::Camera() {
	pos = glm::vec3(0,0,0);
	forward = glm::vec3(1,0,0);
	up = glm::vec3(0,1,0);
	view = glm::mat4(1);
	view = glm::translate(view, pos);
	fov = 3.14159f / 2;
}

Camera::Camera(glm::vec3 _pos, glm::vec3 _forward, glm::vec3 _up) {
	pos = _pos;
	forward = _forward;
	up = _up;
	fov = 3.14159f / 2;
}


glm::vec3 Camera::side() {
	return glm::cross(forward, up);
}


glm::vec3 Camera::getPos() {
	return pos;
}

void Camera::translateRelative(glm::vec3 amount) {
	view = glm::translate(view, amount);
}

void Camera::rotateRelative(glm::vec3 amount) {
	glm::vec3 axisx = view * glm::vec4(1, 0, 0, 0);
	glm::vec3 axisy = view * glm::vec4(0, 1, 0, 0);
	glm::vec3 axisz = view * glm::vec4(0, 0, 1, 0);
	view = view * (glm::rotate(glm::mat4(1), amount.y, axisy) * glm::rotate(glm::mat4(1), amount.x, axisx) * glm::rotate(glm::mat4(1), amount.z, axisz));
	//view = glm::mat4(1);
	//yaw
	/*view = glm::rotate(view, amount.y, up);
	forward = glm::rotate(forward, amount.y, up);
	//pitch
	glm::vec3 side = side();
	view = glm::rotate(view, amount.x, side);
	forward = glm::rotate(forward, amount.x, up);*/
}

void Camera::set(glm::vec3 pos, glm::vec3 angles) {
	glm::vec3 axisx = glm::vec4(1, 0, 0, 0);
	glm::vec3 axisy = glm::vec4(0, 1, 0, 0);
	glm::vec3 axisz = glm::vec4(0, 0, 1, 0);
	view = glm::translate(glm::mat4(1), pos) * (glm::rotate(glm::mat4(1), angles.y, axisy) * glm::rotate(glm::mat4(1), angles.x, axisx) * glm::rotate(glm::mat4(1), angles.z, axisz));
}

void Camera::interpolate(Camera* a, Camera* b, float r) {
	float mr = 1 - r;
	pos = a->getPos()*mr + b->getPos()*r;
	pos = a->getPos()*mr + b->getPos()*r;
}


void Camera::setFoV(float val) {
	fov = val;
}

float Camera::getFoV() {
	return fov;
}

glm::mat4 Camera::getView() {
	glm::mat4 ret = glm::inverse(view);
	//recalculate
	return ret;
}