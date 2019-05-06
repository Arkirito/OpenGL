#include "Lights\DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 position, float nearPlane, float farPlane)
{
	mDirection = direction;
	mPosition = position;
	near_plane = nearPlane;
	far_plane = farPlane;

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(position,
		position + direction,
		glm::vec3(0.0f, 1.0f, 0.0f));
	mLightSpaceMatrix = lightProjection * lightView;
}


DirectionalLight::~DirectionalLight()
{
}

glm::mat4 DirectionalLight::GetLightSpaceMatrix()
{
	return mLightSpaceMatrix;
}

glm::vec3 DirectionalLight::GetDirection()
{
	return mDirection;
}
