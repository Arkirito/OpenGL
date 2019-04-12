#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 pos, glm::vec3 targetPos);

	void SetPosition(glm::vec3 pos);
	void SetTargetPosition(glm::vec3 targetPos);

	glm::mat4 GetViewMatrix();

protected:

	glm::vec3 mPosition;
	glm::vec3 mCameraTarget;
	glm::vec3 mCameraDirection;
	glm::vec3 mCameraUp;
	glm::vec3 mCameraRight;

	glm::mat4 mViewMatrix;

	// Based on mPosition and mCameraTarget (and the up vector witch is currently hardcoded)
	void UpdateViewMatrix();
};

