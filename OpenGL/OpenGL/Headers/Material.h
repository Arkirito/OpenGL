#pragma once
#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	Material();
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;
};

