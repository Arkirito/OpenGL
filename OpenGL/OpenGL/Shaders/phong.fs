#version 330 core
out vec4 FragColor;
in vec4 vertexColor; 

in vec2 TexCoord;

in vec3 FragPos;  
in vec3 Normal; 

uniform sampler2D ourTexture;

uniform vec3 viewPos;
uniform vec3 lightPos; 
uniform vec3 lightColor; 

void main()
{
    float ambientStrength = 0.7;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 5.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  
	// vertexColor is not included for better picture with the current data
    FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(ourTexture, TexCoord);
} 