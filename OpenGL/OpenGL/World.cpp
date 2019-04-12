#include "World.h"



World::World()
{
}


World::~World()
{
}

void World::RegisterObject(WorldObject * obj)
{
	mObjects.push_back(obj);
}

void World::UnregisterObject(WorldObject * obj)
{
	std::vector<WorldObject*>::iterator iter;

	for (iter = mObjects.begin(); iter < mObjects.end(); ++iter)
	{
		if (*iter == obj)
		{
			mObjects.erase(iter);
			return;
		}
	}
}

void World::HandleInput(GLFWwindow * window)
{
	for (WorldObject* obj : mObjects)
	{
		obj->ProcessInput(window);
	}
}
