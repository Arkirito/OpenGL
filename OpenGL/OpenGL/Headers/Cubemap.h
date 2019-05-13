#pragma once

#include "Texture.h"

#include <vector>

class Cubemap
{
public:
	Cubemap();
	Cubemap(std::vector < std::string > facesPathes);
	unsigned int GetID() const;

private:

	unsigned int mTexID;
};

