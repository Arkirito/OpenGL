#pragma once

#include <string>
#include <vector>

class Texture
{
public:
	void Bind();

	unsigned int GetID() const;
	std::string GetTextureType() const;
	
	void SetTextureType(std::string type);
	void SetTexturePath(std::string path);

	static Texture* LoadTexture(const std::string& filePath);
	static void EmptyTexturePool();

private:

	unsigned int texID;

	std::string mType;
	std::string mPath;

	Texture(const std::string& filePath);

	static std::vector<Texture*> textures_loaded;
};

