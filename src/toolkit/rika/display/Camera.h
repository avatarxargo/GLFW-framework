/**
 * Keeps track of camera variables.
 *
 * @date: 4/18/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
private:
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
	glm::mat4 view;
	glm::vec3 side();
	float fov;
public:
	glm::vec3 getPos();
	void translateRelative(glm::vec3 amount);
	void rotateRelative(glm::vec3 amount);
	void set(glm::vec3 pos, glm::vec3 angles);
	void setFoV(float val);
	float getFoV();
	glm::mat4 getView();
	void interpolate(Camera* a, Camera* b, float rate);
	Camera();
	Camera(glm::vec3 _pos, glm::vec3 _forward, glm::vec3 _up);
	~Camera();
};

#endif
