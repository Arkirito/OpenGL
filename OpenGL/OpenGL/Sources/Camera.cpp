#include "Camera.h"

#include <glfw3.h>

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

glm::mat4 Camera::GetViewMatrix() const
{
	return mViewMatrix;
}

glm::vec3 Camera::GetPosition() const 
{
	return mPosition;
}

void Camera::UpdateViewMatrix()
{
	mCameraDirection = glm::normalize(mPosition - mCameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	mCameraRight = glm::normalize(glm::cross(up, mCameraDirection));
	mCameraUp = glm::cross(mCameraDirection, mCameraRight);

	mViewMatrix = glm::lookAt(mPosition, mCameraTarget, mCameraUp);
}

void Camera::ProcessInput(GLFWwindow * window)
{
	float cameraSpeed = 0.2f; // adjust accordingly
	float speedBoost = 1.f;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speedBoost = 4.f;
	}

	glm::vec3 cachedPost = mPosition;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mPosition += cameraSpeed * speedBoost * mCameraDirection;
		mCameraTarget += cameraSpeed * speedBoost * mCameraDirection;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mPosition -= cameraSpeed * speedBoost * mCameraDirection;
		mCameraTarget -= cameraSpeed * speedBoost * mCameraDirection;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mPosition += cameraSpeed * speedBoost * mCameraRight;
		mCameraTarget += cameraSpeed * speedBoost * mCameraRight;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mPosition -= cameraSpeed * speedBoost * mCameraRight;
		mCameraTarget -= cameraSpeed * speedBoost * mCameraRight;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		mPosition -= glm::normalize(glm::cross(mCameraDirection, mCameraUp)) * cameraSpeed* speedBoost;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		mPosition += glm::normalize(glm::cross(mCameraDirection, mCameraUp)) * cameraSpeed* speedBoost;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mPosition -= glm::normalize(glm::cross(mCameraDirection, mCameraRight)) * cameraSpeed* speedBoost;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mPosition += glm::normalize(glm::cross(mCameraDirection, mCameraRight)) * cameraSpeed* speedBoost;

	if (mPosition != cachedPost)
	{
		UpdateViewMatrix();
	}
}
