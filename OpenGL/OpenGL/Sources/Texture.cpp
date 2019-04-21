#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <vector>

std::vector<Texture*> Texture::textures_loaded;

Texture* Texture::LoadTexture(const std::string& filePath)
{
	Texture* outTexture;
	bool found = false;
	unsigned int j = 0;

	for (; j < textures_loaded.size(); j++)
	{
		if ((std::strcmp(textures_loaded[j]->mPath.data(), filePath.c_str())) == 0)
		{
			outTexture = textures_loaded[j];
			found = true;
			break;
		}
	}

	if (found)
	{
		return textures_loaded[j];
	}

	return new Texture(filePath);
}

void Texture::EmptyTexturePool()
{
	for (unsigned int j = 0; j < textures_loaded.size(); j++)
	{
		delete textures_loaded[j];
	}

	textures_loaded.empty();
}

Texture::Texture(const std::string& filePath)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, texID);
}

unsigned int Texture::GetID() const
{
	return texID;
}

std::string Texture::GetTextureType() const
{
	return mType;
}

void Texture::SetTextureType(std::string type)
{
	mType = type;
}

void Texture::SetTexturePath(std::string path)
{
	mPath = path;
}