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

	class Camera* GetCurrentCamera();

	void Draw(class Shader* shaderToDraw);

	class DirectionalLight* GetDirectionalLight();

protected:

	std::vector<WorldObject*> mObjects;

	class Camera* mCurrentCamera;

	// Default scene objects

	class DirectionalLight* mDirectionalLight; // Only one per scene, casts shadows
};

