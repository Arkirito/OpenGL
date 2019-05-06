#pragma once
#include "Light.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 direction, glm::vec3 position, float near, float far);
	virtual ~DirectionalLight();

	glm::mat4 GetLightSpaceMatrix();
	glm::vec3 GetDirection();
	
	bool CastShadows = true;

protected:
	glm::vec3 mDirection;

	// Used for shadows
	glm::vec3 mPosition;
	float near_plane = 1.f;
	float far_plane = 100.f;

	glm::mat4 mLightSpaceMatrix;
};

