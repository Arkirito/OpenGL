#pragma once

#include <string>
#include <glad/glad.h> // include glad to get all the required OpenGL headers

class Shader
{
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// use/activate the shader
	void Use();
	// utility uniform functions
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
};

