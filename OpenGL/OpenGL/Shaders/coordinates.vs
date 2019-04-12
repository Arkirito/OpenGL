#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 PVM;

void main()
{
    TexCoord = aTexCoord;
	vertexColor = aColor;
	gl_Position = PVM * vec4(aPos, 1.0);
}