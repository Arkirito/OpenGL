#pragma once
#include "WorldObject.h"
#include "Material.h"

class Light : public WorldObject
{
public:
	Light();
	virtual ~Light();

	Material mMaterial;

	void ProcessInput(struct GLFWwindow *window);
};

