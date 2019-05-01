#include <glfw3.h>

#include "GlobalInstance.h"

#include "World.h"
#include "Settings.h"

GlobalInstance* GlobalInstance::mInstance;

GlobalInstance::GlobalInstance()
{
	mWorld = new World();
	mSettings = new Settings();
}

GlobalInstance::~GlobalInstance()
{
	delete mWorld;
	delete mSettings;
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

const Settings * GlobalInstance::GetSettings() const
{
	return mSettings;
}
