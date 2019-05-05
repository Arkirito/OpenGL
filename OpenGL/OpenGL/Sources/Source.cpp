#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <random>

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

int config = 0;

/*float vertices[] = {

	// positions        // colors                // texture coords
	0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // top right
	0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f  // top left 
};*/

/*float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 0.0f, 0.f, 0.f, -1.f,
	0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 0.0f, 0.f, 0.f, -1.f,
	0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 1.0f, 0.f, 0.f, -1.f,
	0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 1.0f, 0.f, 0.f, -.1f,
	-0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 1.0f, 0.f, 0.f, -1.f,
	-0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 0.0f, 0.f, 0.f, -1.f,

	-0.5f, -0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 0.0f, 0.f, 0.f, 1.f,
	0.5f, -0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 0.0f, 0.f, 0.f, 1.f,
	0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 1.0f, 0.f, 0.f, 1.f,
	0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 1.0f, 0.f, 0.f, 1.f,
	-0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 1.0f, 0.f, 0.f, 1.f,
	-0.5f, -0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 0.0f, 0.f, 0.f, 1.f,

	-0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 0.0f, -1.f, 0.f, 0.f,
	-0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 1.0f, -1.f, 0.f, 0.f,
	-0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 1.0f, -1.f, 0.f, 0.f,
	-0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 1.0f, -1.f, 0.f, 0.f,
	-0.5f, -0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 0.0f, -1.f, 0.f, 0.f,
	-0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 0.0f, -1.f, 0.f, 0.f,

	0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 0.0f, 1.f, 0.f, 0.f,
	0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 1.0f, 1.f, 0.f, 0.f,
	0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 1.0f, 1.f, 0.f, 0.f,
	0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 1.0f, 1.f, 0.f, 0.f,
	0.5f, -0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 0.0f, 1.f, 0.f, 0.f,
	0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 0.0f, 1.f, 0.f, 0.f,

	-0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 1.0f, 0.f, -1.f, 0.f,
	0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 1.0f, 0.f, -1.f, 0.f,
	0.5f, -0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 0.0f, 0.f, -1.f, 0.f,
	0.5f, -0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 0.0f, 0.f, -1.f, 0.f,
	-0.5f, -0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 0.0f, 0.f, -1.f, 0.f,
	-0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 1.0f, 0.f, -1.f, 0.f,

	-0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 1.0f, 0.f, 1.f, 0.f,
	0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 1.0f, 0.f, 1.f, 0.f,
	0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 0.0f, 0.f, 1.f, 0.f,
	0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  1.0f, 0.0f, 0.f, 1.f, 0.f,
	-0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 0.0f, 0.f, 1.f, 0.f,
	-0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.f, 1.0f,  0.0f, 1.0f, 0.f, 1.f, 0.f
};*/

/*glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};*/

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

unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
						 // positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
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

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		config = 0;
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		config = 1;
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

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

	GLFWwindow* window = glfwCreateWindow(settings.ViewportWidth, settings.ViewportHeight, "LearnOpenGL", NULL, NULL);
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

	// Shaders
	Shader baseShader("Shaders/Mesh.vs", "Shaders/Mesh.fs");
	Shader coloredShader("Shaders/SingleColorMesh.vs", "Shaders/SingleColorMesh.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader reflectionShader("Shaders/reflection.vs", "Shaders/reflection.fs");
	Shader refractionShader("Shaders/reflection.vs", "Shaders/refraction.fs");
	Shader shadowDepthShader("Shaders/shadowDepth.vs", "Shaders/shadowDepth.fs");

	// Post-Process
	Shader screenShader("Shaders/screenShader.vs", "Shaders/screenShader.fs");
	Shader negativeShader("Shaders/screenShader.vs", "Shaders/negative.fs");
	Shader grayscaleShader("Shaders/screenShader.vs", "Shaders/grayscale.fs");
	Shader kernelShader("Shaders/screenShader.vs", "Shaders/kernel.fs");
	Shader blurShader("Shaders/screenShader.vs", "Shaders/blur.fs"); //TODO: blur is a kernel effect too, maybe there is a reason to create a unified solution here 
	Shader exposureToneMappingShader("Shaders/screenShader.vs", "Shaders/exposureToneMapping.fs");
	Shader gaussianBlur("Shaders/screenShader.vs", "Shaders/gaussianBlur.fs");
	Shader bloom("Shaders/screenShader.vs", "Shaders/bloom.fs"); // just blends blur+brightness
	Shader defferedShading("Shaders/screenShader.vs", "Shaders/defferedShading.fs");
	Shader debugShadowDepth("Shaders/screenShader.vs", "Shaders/pp_shadowDepth.fs");
	Shader shaderSSAO("Shaders/screenShader.vs", "Shaders/shaderSSAO.fs");

	Camera camera(glm::vec3(0.0f, 8.0f, 25.0f), glm::vec3(0.0f, 8.0f, 0.0f));

	Model model("Content/Models/nanosuit/nanosuit.obj");

	Cubemap cubemap(faces);

	Texture* cubeDiffuse = Texture::LoadTexture("Content/Textures/container_diffuse.jpg");
	Texture* cubeSpecular = Texture::LoadTexture("Content/Textures/container_specular.jpg");

	Texture* metalDiffuse = Texture::LoadTexture("Content/Textures/metalPBR/rustediron2_basecolor.png");
	Texture* metalMetallic = Texture::LoadTexture("Content/Textures/metalPBR/rustediron2_metallic.png");
	Texture* metalNormal = Texture::LoadTexture("Content/Textures/metalPBR/rustediron2_normal.png");
	Texture* metalRoughness = Texture::LoadTexture("Content/Textures/metalPBR/rustediron2_roughness.png");

	//wood
	Texture* woodenFloor_Diffuse = Texture::LoadTexture("Content/Textures/woodenFloorPBR/hardwood-brown-planks-albedo.png");
	Texture* woodenFloor_Height = Texture::LoadTexture("Content/Textures/woodenFloorPBR/hardwood-brown-planks-height.png");
	Texture* woodenFloor_Normal = Texture::LoadTexture("Content/Textures/woodenFloorPBR/hardwood-brown-planks-normal-dx.png");
	Texture* woodenFloor_Metallic = Texture::LoadTexture("Content/Textures/woodenFloorPBR/hardwood-brown-planks-metallic.png");

	//parallax
	Texture* brickWall_Diffuse = Texture::LoadTexture("Content/Textures/parallaxBricks/diffuse.jpg");
	Texture* brickWall_Normal = Texture::LoadTexture("Content/Textures/parallaxBricks/normals.jpg");
	Texture* brickWall_Height = Texture::LoadTexture("Content/Textures/parallaxBricks/height.jpg");

	// framebuffer configuration
	// -------------------------
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffers[5]; // 0 - color, 1 - brightness, 2 - position, 3 - normals, 4 - specular
	glGenTextures(5, textureColorbuffers);
	for (unsigned int i = 0; i < 5; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textureColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, settings.ViewportWidth, settings.ViewportHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureColorbuffers[i], 0);
	}

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, settings.ViewportWidth, settings.ViewportHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, GlobalInstance::GetInstance()->GetSettings()->ViewportWidth, GlobalInstance::GetInstance()->GetSettings()->ViewportHeight, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
		);
	}

	srand(time(NULL));

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	for (int i = 0; i < 10; ++i)
	{
		lightPositions.push_back(glm::vec3(rand() % 20 - 10, rand() % 2, rand() % 20 - 10));
		lightColors.push_back(glm::vec3(rand() % 10, rand() % 10, rand() % 10));
	}

	// Shadow
	unsigned int depthMapFBO;
	unsigned int depthMap;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	{
		glGenFramebuffers(1, &depthMapFBO);

		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//SSAO Kernel

	std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);

		float scale = (float)i / 64.0;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	unsigned int noiseTexture;
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned int ssaoFBO;
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	unsigned int ssaoColorBuffer;
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, settings.ViewportWidth, settings.ViewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		GlobalInstance::GetInstance()->HandleInput(window);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_CULL_FACE);

		// Shadow rendering
		{
			// 1. first render to depth map
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_FRONT);
			DrawScene(shadowDepthShader, camera, brickWall_Diffuse, brickWall_Height, brickWall_Normal, brickWall_Height, depthMap);
			glCullFace(GL_BACK);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		}
		
		glViewport(0, 0, settings.ViewportWidth, settings.ViewportHeight);

		// rendering
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glDepthFunc(GL_LESS);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);

		unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		glDrawBuffers(5, attachments);

		baseShader.Use();

		// shadow
		{
			float near_plane = 0.2f, far_plane = 40.5f;
			glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			glm::mat4 lightView = glm::lookAt(glm::vec3(4.0f, 4.0f, 14.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 lightSpaceMatrix = lightProjection * lightView;

			baseShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		}

		// Coordinates

		baseShader.SetVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		baseShader.SetVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken the light a bit to fit the scene
		baseShader.SetVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		baseShader.SetVec3("dirLight.direction", glm::vec3(-1.0f, -1.0f, -1.0f));

		baseShader.SetVec3("spotLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		baseShader.SetVec3("spotLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken the light a bit to fit the scene
		baseShader.SetVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		baseShader.SetVec3("spotLight.direction", glm::vec3(-1.0f, -1.0f, -1.0f));
		baseShader.SetVec3("spotLight.position", camera.GetPosition());
		baseShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		baseShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(22.5f)));

		for (GLuint i = 0; i < 4; i++)
		{
			std::string number = std::to_string(i);

			baseShader.SetVec3(("pointLights[" + number + "].position").c_str(), glm::vec3(pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z));
			baseShader.SetVec3(("pointLights[" + number + "].ambient").c_str(), glm::vec3(pointLightColors[i].r * 0.1f, pointLightColors[i].g * 0.1f, pointLightColors[i].b * 0.1f));
			baseShader.SetVec3(("pointLights[" + number + "].diffuse").c_str(), glm::vec3(pointLightColors[i].r, pointLightColors[i].g, pointLightColors[i].b));
			baseShader.SetVec3(("pointLights[" + number + "].specular").c_str(), glm::vec3(1.0f, 1.0f, 1.0f));
			baseShader.SetFloat(("pointLights[" + number + "].constant").c_str(), 1.0f);
			baseShader.SetFloat(("pointLights[" + number + "].linear").c_str(), 0.09f);
			baseShader.SetFloat(("pointLights[" + number + "].quadratic").c_str(), 0.032f);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Lights
		/*{
			// spot lights
			coloredShader.Use();
			coloredShader.SetVec4("mColor", glm::vec4(10.5f, 10.5f, 10.5f, 1.f));
			PrimitiveManager::DrawCube(coloredShader, camera, camera.GetPosition(), glm::vec3(0, 0, 0), glm::vec3(0.3, 0.3, 0.3), cubeDiffuse, cubeSpecular);

			// spoint lights
			for (GLuint i = 0; i < 4; i++)
			{
				coloredShader.SetVec4("mColor", glm::vec4(pointLightColors[i].r, pointLightColors[i].g, pointLightColors[i].b, 1.f));
				PrimitiveManager::DrawCube(coloredShader, camera, glm::vec3(pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z), glm::vec3(0, 0, 0), glm::vec3(1.0, 1.0, 1.0), cubeDiffuse, cubeSpecular);
			}
		}*/

		baseShader.Use();

		DrawScene(baseShader, camera, brickWall_Diffuse, brickWall_Height, brickWall_Normal, brickWall_Height, depthMap);

		//PrimitiveManager::DrawCube(baseShader, camera, glm::vec3(glm::sin(glfwGetTime()/2) * 10, 2, glm::cos(glfwGetTime()/2) * 10), glm::vec3(0, 0, 0), glm::vec3(2, 2, 2), cubeDiffuse, cubeSpecular);
		//model.Draw(baseShader);

		//PrimitiveManager::DrawSkybox(skyboxShader, camera, cubemap);

		bool horizontal = true, first_iteration = true;
		int amount = 10;
		gaussianBlur.Use();
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			gaussianBlur.SetInt("horizontal", horizontal);
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? textureColorbuffers[1] : pingpongBuffer[!horizontal]
			);

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
								  // clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);



		//screenShader.Use();

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffers[0]);	// use the color attachment texture as the texture of the quad plane
		bloom.SetInt("screenTexture", 0);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);*/

	    // Post process shader:
		/*bloom.Use();
		bloom.SetFloat("exposure", 0.2f);
		glBindVertexArray(quadVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffers[1]);	// use the color attachment texture as the texture of the quad plane
		bloom.SetInt("screenTexture", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);e
		bloom.SetInt("brightnessTexture", 1);

		glDrawArrays(GL_TRIANGLES, 0, 6);*/

		// use G-buffer to render SSAO texture
		// 2. generate SSAO texture
		// ------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSSAO.Use();
		// Send kernel + rotation 
		for (unsigned int i = 0; i < 64; ++i)
			shaderSSAO.SetVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffers[2]);
		shaderSSAO.SetInt("gPosition", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffers[3]);
		shaderSSAO.SetInt("gNormal", 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		shaderSSAO.SetInt("texNoise", 2);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), settings.ViewportWidth / settings.ViewportHeight, 0.1f, 100.0f);
		shaderSSAO.SetMat4("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*debugShadowDepth.Use();
		//debugShadowDepth.SetFloat("exposure", 0.1f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);	// use the color attachment texture as the texture of the quad plane
		debugShadowDepth.SetInt("screenTexture", 0);
		debugShadowDepth.SetFloat("near_plane", 1.f);
		debugShadowDepth.SetFloat("far_plane", 40.5f);
		glDrawArrays(GL_TRIANGLES, 0, 6);*/

		/*			screenShader.Use();
			screenShader.SetFloat("exposure", 0.2f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
			screenShader.SetInt("screenTexture", 0);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;*/

		switch (config)
		{
		case 1:
		case 0:
		default:
			defferedShading.Use();

			defferedShading.SetVec3("viewPos", camera.GetPosition());
			defferedShading.SetFloat("exposure", 0.1f);

			glBindVertexArray(quadVAO);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffers[2]);
			defferedShading.SetInt("gPosition", 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffers[3]);
			defferedShading.SetInt("gNormal", 1);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffers[4]);
			defferedShading.SetInt("gAlbedoSpec", 2);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
			defferedShading.SetInt("ssao", 3);

			for (int i = 0; i < 10; ++i)
			{
				defferedShading.SetVec3(("lights[" + std::to_string(i) + "].Position").c_str(), lightPositions[i]);
				defferedShading.SetVec3(("lights[" + std::to_string(i) + "].Color").c_str(), lightColors[i]);
			}

			defferedShading.SetInt("useSSAO", config);

			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Texture::EmptyTexturePool();

	glfwTerminate();
	return 0;
}