#pragma once
class WorldObject
{
public:
	WorldObject();
	virtual ~WorldObject();


	virtual void ProcessInput(struct GLFWwindow *window) = 0;
};

