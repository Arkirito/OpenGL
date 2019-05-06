#include "GlobalInstance.h"

//#include <glfw3.h>

#include "World.h"
#include "Settings.h"
#include "Renderer.h"

GlobalInstance* GlobalInstance::mInstance;

GlobalInstance::GlobalInstance()
{
	mSettings = new Settings();
}

GlobalInstance::~GlobalInstance()
{
	if (mWorld)
	{
		delete mWorld;
	}

	if (mRenderer)
	{
		delete mRenderer;
	}

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
	mRenderer->HandleInput(window);
}

const Settings * GlobalInstance::GetSettings() const
{
	return mSettings;
}

Renderer * GlobalInstance::GetRenderer()
{
	return mRenderer;
}

void GlobalInstance::Tick(GLFWwindow * window, float dt)
{
	HandleInput(window);
	mRenderer->Render(dt);
}

void GlobalInstance::Init()
{
	mWorld = new World();
	mRenderer = new Renderer();
}
