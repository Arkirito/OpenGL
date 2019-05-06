#pragma once
#include <memory>

class World;
class Settings;
class Renderer;

class GlobalInstance
{
public:

	static GlobalInstance* GetInstance();

	World* GetWorld();

	void HandleInput(struct GLFWwindow *window);

	const Settings* GetSettings() const;

	Renderer* GetRenderer();

	void Tick(struct GLFWwindow *window, float dt);

	void Init();

protected:

	World* mWorld;
	Renderer* mRenderer;
	Settings* mSettings;

private:

	static GlobalInstance* mInstance;

	GlobalInstance();
	~GlobalInstance();
};

