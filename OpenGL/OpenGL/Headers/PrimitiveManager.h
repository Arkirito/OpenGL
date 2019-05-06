#pragma once

#include <glm/glm.hpp>

class PrimitiveManager
{
public:
	static void DrawCube(class Shader* shader, class Camera* camera, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, 
		class Texture * diffuse = nullptr, 
		class Texture * specular = nullptr);
	static void DrawQuad(class Shader & shader, class Camera & camera, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, 
		class Texture * diffuse, 
		class Texture * specular, 
		class Texture * normal,
		class Texture * depth,
		unsigned int shadow);
	static void DrawSkybox(class Shader & shader,class Camera & camera,class Cubemap& cubemap);

protected:

	static void Setup();

	static void SetupCube();
	static void SetupQuad();
	static void SetupSkybox();

	static unsigned int cubeVAO;
	static unsigned int cubeVBO;
	static unsigned int quadVAO;
	static unsigned int quadVBO;
	static unsigned int skyboxVAO;
	static unsigned int skyboxVBO;
};

