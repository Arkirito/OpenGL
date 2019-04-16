#pragma once
#include <memory>

class World;

class GlobalInstance
{
public:

	static GlobalInstance* GetInstance();

	World* GetWorld();

	void HandleInput(struct GLFWwindow *window);

protected:

	World* mWorld;

private:

	static GlobalInstance* mInstance;

	GlobalInstance();
	~GlobalInstance();
};

