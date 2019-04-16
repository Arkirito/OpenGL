#include <glfw3.h>

#include "GlobalInstance.h"

#include "World.h"

GlobalInstance* GlobalInstance::mInstance;

GlobalInstance::GlobalInstance()
{
	mWorld = new World();
}

GlobalInstance::~GlobalInstance()
{
	delete mWorld;
}

GlobalInstance * GlobalInstance::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new GlobalInstance();
	}

	return mInstance;
}

World* GlobalInstance::GetWorld()
{
	return mWorld;
}

void GlobalInstance::HandleInput(GLFWwindow * window)
{
	mWorld->HandleInput(window);
}
