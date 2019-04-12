#include "Camera.h"

Camera::Camera()
{
	mPosition = glm::vec3(0.f, 0.f, 0.f);
	mCameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	UpdateViewMatrix();
}

Camera::Camera(glm::vec3 pos, glm::vec3 targetPos)
{
	mPosition = pos;
	mCameraTarget = targetPos;

	UpdateViewMatrix();
}

void Camera::SetPosition(glm::vec3 pos)
{
	mPosition = pos;

	UpdateViewMatrix();
}

void Camera::SetTargetPosition(glm::vec3 targetPos)
{
	mCameraTarget = targetPos;

	UpdateViewMatrix();
}

glm::mat4 Camera::GetViewMatrix()
{
	return mViewMatrix;
}

void Camera::UpdateViewMatrix()
{
	mCameraDirection = glm::normalize(mPosition - mCameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	mCameraRight = glm::normalize(glm::cross(up, mCameraDirection));
	mCameraUp = glm::cross(mCameraDirection, mCameraRight);

	mViewMatrix = glm::lookAt(mPosition, mCameraTarget, mCameraUp);
}
