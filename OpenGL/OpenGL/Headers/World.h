#pragma once
#include <vector>

#include "WorldObject.h"

class World
{
public:
	World();
	virtual ~World();

	void RegisterObject(WorldObject* obj);
	void UnregisterObject(WorldObject* obj);

	void HandleInput(struct GLFWwindow *window);

protected:

	std::vector<WorldObject*> mObjects;
};

