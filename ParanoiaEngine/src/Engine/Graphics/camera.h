#pragma once
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera(float left, float right, float bottom, float top);

	void setPosition(const glm::vec3& position);

	const glm::mat4& getViewProjectionMatrix() const;

private:
	void recalculate();

	glm::mat4 projection_matrix;
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::mat4 view_matrix = { 1.0f };
	glm::mat4 view_projection_matrix;
};