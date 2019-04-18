#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec4 vertexColor;
out vec2 TexCoords;

out vec3 FragPos;  
out vec3 Normal;

uniform mat4 PVM;
uniform mat4 uModel;

void main()
{
    TexCoords = aTexCoord;
	vertexColor = aColor;

	FragPos = vec3(uModel * vec4(aPos, 1.0));
    Normal = aNormal;

	gl_Position = PVM * vec4(aPos, 1.0);
}