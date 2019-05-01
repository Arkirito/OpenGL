#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;

out vec3 FragPos;  
out vec3 Normal;
out mat3 TBN;

uniform mat4 PVM;
uniform mat4 uModel;

out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform vec3 viewPos;

void main()
{
    FragPos = vec3(uModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(uModel))) * aNormal;  
    TexCoords = aTexCoords;

    vec3 T = normalize(mat3(uModel) * aTangent);
    vec3 B = normalize(mat3(uModel) * aBitangent);
    vec3 N = normalize(mat3(uModel) * aNormal);
    TBN = transpose(mat3(T, B, N));
    
	TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;

	gl_Position = PVM * vec4(aPos, 1.0);
}