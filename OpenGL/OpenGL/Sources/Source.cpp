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
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
	glm::vec3(1.7f,  1.2f,  1.0f),
	glm::vec3(2.3f,  3.3f, 4.0f),
	glm::vec3(4.0f,  2.0f, 12.0f),
	glm::vec3(1.0f,  1.0f, 3.0f)
};

unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
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

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height); });

	// High Level Stuff
	Shader baseShader("Shaders/Mesh.vs", "Shaders/Mesh.fs");
	Camera camera(glm::vec3(0.0f, 8.0f, 25.0f), glm::vec3(0.0f, 8.0f, 0.0f));

	Model model("Content/Models/nanosuit/nanosuit.obj");

	while (!glfwWindowShouldClose(window))
	{
		GlobalInstance::GetInstance()->HandleInput(window);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		baseShader.Use();

		// Coordinates

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		float angle = 20.0f;
		//modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);

		glm::mat4 PVM = projection * camera.GetViewMatrix() * modelMatrix;

		baseShader.SetMat4("PVM", PVM);
		baseShader.SetMat4("uModel", modelMatrix);

		baseShader.SetVec3("dirLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		baseShader.SetVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken the light a bit to fit the scene
		baseShader.SetVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		baseShader.SetVec3("dirLight.direction", glm::vec3(-1.0f, -1.0f, -1.0f));

		baseShader.SetVec3("spotLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
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

		model.Draw(baseShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Texture::EmptyTexturePool();

	glfwTerminate();
	return 0;
}