#version 330 core
out vec4 FragColor;
in vec4 vertexColor; 

in vec2 TexCoord;

in vec3 FragPos;  
in vec3 Normal; 

uniform sampler2D ourTexture;
uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 ambient;
    vec3 position;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoord));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoord));
	
	// vertexColor is not included for better picture with the current data
    FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(ourTexture, TexCoord);
} 