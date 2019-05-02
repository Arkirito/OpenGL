#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>

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

#define ViewportWidh 1600.f
#define ViewportHeight 720.f

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
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS

	GLFWwindow* window = glfwCreateWindow(ViewportWidh, ViewportHeight, "LearnOpenGL", NULL, NULL);
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

	glViewport(0, 0, ViewportWidh, ViewportHeight);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height); });

	// Shaders
	Shader baseShader("Shaders/Mesh.vs", "Shaders/Mesh.fs");
	Shader coloredShader("Shaders/SingleColorMesh.vs", "Shaders/SingleColorMesh.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader reflectionShader("Shaders/reflection.vs", "Shaders/reflection.fs");
	Shader refractionShader("Shaders/reflection.vs", "Shaders/refraction.fs");

	// Post-Process
	Shader screenShader("Shaders/screenShader.vs", "Shaders/screenShader.fs");
	Shader negativeShader("Shaders/screenShader.vs", "Shaders/negative.fs");
	Shader grayscaleShader("Shaders/screenShader.vs", "Shaders/grayscale.fs");
	Shader kernelShader("Shaders/screenShader.vs", "Shaders/kernel.fs");
	Shader blurShader("Shaders/screenShader.vs", "Shaders/blur.fs"); //TODO: blur is a kernel effect too, maybe there is a reason to create a unified solution here 
	Shader exposureToneMappingShader("Shaders/screenShader.vs", "Shaders/exposureToneMapping.fs");

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
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ViewportWidh, ViewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ViewportWidh, ViewportHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
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

	while (!glfwWindowShouldClose(window))
	{
		GlobalInstance::GetInstance()->HandleInput(window);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_CULL_FACE);

		glDepthFunc(GL_LESS);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);

		baseShader.Use();

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
		{
			// spot lights
			coloredShader.Use();
			coloredShader.SetVec4("mColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
			PrimitiveManager::DrawCube(coloredShader, camera, camera.GetPosition(), glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1), cubeDiffuse, cubeSpecular);

			// spoint lights
			for (GLuint i = 0; i < 4; i++)
			{
				coloredShader.SetVec4("mColor", glm::vec4(pointLightColors[i].r, pointLightColors[i].g, pointLightColors[i].b, 1.f));
				PrimitiveManager::DrawCube(coloredShader, camera, glm::vec3(pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z), glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1), cubeDiffuse, cubeSpecular);
			}
		}

		PrimitiveManager::DrawQuad(baseShader, camera, glm::vec3(0, 0, 0), glm::vec3(-90, 0, 0), glm::vec3(15, 15, 1), brickWall_Diffuse, brickWall_Height, brickWall_Normal, brickWall_Height);
		//PrimitiveManager::DrawCube(baseShader, camera, glm::vec3(glm::sin(glfwGetTime()/2) * 10, 2, glm::cos(glfwGetTime()/2) * 10), glm::vec3(0, 0, 0), glm::vec3(2, 2, 2), cubeDiffuse, cubeSpecular);
		//PrimitiveManager::DrawCube(baseShader, camera, glm::vec3(glm::sin(glfwGetTime()) * 10 + 4, 10, glm::cos(glfwGetTime()) * 10 + 8), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), cubeDiffuse, cubeSpecular);
		
		//model.Draw(baseShader);

		PrimitiveManager::DrawSkybox(skyboxShader, camera, cubemap);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
								  // clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		// Post process shader:
		exposureToneMappingShader.Use();
		exposureToneMappingShader.SetFloat("exposure", 0.2f);
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Texture::EmptyTexturePool();

	glfwTerminate();
	return 0;
}