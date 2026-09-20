#include "camera.h"

Camera::Camera(float left, float right, float bottom, float top) {
	projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	Camera::recalculate();
}

void Camera::setPosition(const glm::vec3& position) {
	this->position = position;
	recalculate();
}

const glm::mat4& Camera::getViewProjectionMatrix() const {
	return view_projection_matrix;
}

void Camera::recalculate() {
	view_matrix = glm::translate(glm::mat4(1.0f), -position);
	view_projection_matrix = projection_matrix * view_matrix;
}
