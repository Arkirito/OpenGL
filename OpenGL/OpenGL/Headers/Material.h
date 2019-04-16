#pragma once
#include <vector>

#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	Material();
	virtual ~Material();

protected:

	Shader * mShader;
	std::vector<Texture*> mTextures;
};

