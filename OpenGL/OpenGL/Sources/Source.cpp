#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <random>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Custom

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "World.h"
#include "GlobalInstance.h"
#include "Model.h"
#include "Cubemap.h"
#include "PrimitiveManager.h"
#include "Settings.h"
#include "Lights\DirectionalLight.h"

using namespace std::chrono;

int config = 0;

glm::vec3 pointLightPositions[] = {
	glm::vec3(10.f,  0.2f,  2.0f),
	glm::vec3(2.3f, 3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(-5.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
	glm::vec3(12.7f,  13.2f,  14.0f),
	glm::vec3(22.3f,  43.3f, 41.0f),
	glm::vec3(44.0f,  21.0f, 12.0f),
	glm::vec3(1.0f,  1.0f, 30.0f)
};

std::vector<std::string> faces
{
	"Content/Textures/Skyboxes/Clouds/right.tga",
	"Content/Textures/Skyboxes/Clouds/left.tga",
	"Content/Textures/Skyboxes/Clouds/top.tga",
	"Content/Textures/Skyboxes/Clouds/bottom.tga",
	"Content/Textures/Skyboxes/Clouds/front.tga",
	"Content/Textures/Skyboxes/Clouds/back.tga"
};

void DrawScene(Shader & shader, Camera & camera,
	Texture * diffuse,
	Texture * specular,
	Texture * normal,
	Texture * depth, 
	unsigned int shadow)
{
	PrimitiveManager::DrawQuad(shader, camera, glm::vec3(0, 0, 0), glm::vec3(-90, 0, 0), glm::vec3(15, 15, 1), diffuse, depth, normal, depth, shadow);
	PrimitiveManager::DrawQuad(shader, camera, glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::vec3(15, 15, 1), diffuse, depth, normal, depth, shadow);
}

int main()
{
	const Settings& settings = *GlobalInstance::GetInstance()->GetSettings();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS

	GLFWwindow* window = glfwCreateWindow(static_cast<int>(settings.ViewportWidth), static_cast<int>(settings.ViewportHeight), "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height); });

	GlobalInstance::GetInstance()->Init();

	Camera camera(glm::vec3(0.0f, 8.0f, 25.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	DirectionalLight light(glm::vec3(-5, -5, -5), glm::vec3(5, 5, 5), 1.0, 100);

	light.mMaterial.mAmbient = glm::vec3(0.1, 0.1, 0.1);
	light.mMaterial.mDiffuse = glm::vec3(10.6, 20.6, 5.6);
	light.mMaterial.mSpecular = glm::vec3(1.6, 0.6, 0.6);

	GlobalInstance::GetInstance()->GetWorld()->RegisterObject(&camera);
	GlobalInstance::GetInstance()->GetWorld()->RegisterObject(&light);

	Model model("Content/Models/nanosuit/nanosuit.obj");

	Cubemap cubemap(faces);

	Texture* cubeDiffuse = Texture::LoadTexture("Content/Textures/container_diffuse.jpg");
	Texture* cubeSpecular = Texture::LoadTexture("Content/Textures/container_specular.jpg");

	Texture* metalDiffuse = Texture::LoadTexture("Content/Textures/metalPBR/rustediron2_basecolor.png");
	Texture* metalMetallic = Texture::LoadTexture("Content/Textures/metalPBR/rustediron2_metallic.png");
	Texture* metalNormal = Texture::LoadTexture("Content/Textures/metalPBR/rustediron2_normal.png");
	Texture* metalRoughness = Texture::LoadTexture("Content/Textures/metalPBR/rustediron2_roughness.png");

	//wood


	//parallax
	Texture* brickWall_Diffuse = Texture::LoadTexture("Content/Textures/parallaxBricks/diffuse.jpg");
	Texture* brickWall_Normal = Texture::LoadTexture("Content/Textures/parallaxBricks/normals.jpg");
	Texture* brickWall_Height = Texture::LoadTexture("Content/Textures/parallaxBricks/height.jpg");

	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	while (!glfwWindowShouldClose(window))
	{
		milliseconds current = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		float dt = static_cast<float>((current - ms).count());
		ms = current;

		GlobalInstance::GetInstance()->HandleInput(window);
		GlobalInstance::GetInstance()->Tick(window, dt);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Texture::EmptyTexturePool();

	glfwTerminate();
	return 0;
}