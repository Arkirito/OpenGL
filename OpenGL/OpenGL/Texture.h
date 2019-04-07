#pragma once

#include <string>

class Texture
{
public:
	Texture(const std::string& filePath);
	virtual ~Texture();

	void Bind();

private:

	unsigned char *mData;

	unsigned int texID;
};

