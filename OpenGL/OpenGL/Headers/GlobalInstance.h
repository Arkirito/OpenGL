#pragma once
#include <memory>

class World;
class Settings;

class GlobalInstance
{
public:

	static GlobalInstance* GetInstance();

	World* GetWorld();

	void HandleInput(struct GLFWwindow *window);

	const Settings* GetSettings() const;

protected:

	World* mWorld;

	Settings* mSettings;

private:

	static GlobalInstance* mInstance;

	GlobalInstance();
	~GlobalInstance();
};

