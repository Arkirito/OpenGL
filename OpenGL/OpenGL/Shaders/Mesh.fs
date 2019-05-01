#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

in vec3 FragPos;  
in vec3 Normal; 
in mat3 TBN;

in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D ourTexture;
//uniform vec3 viewPos;

struct Material {
    float shininess;

	sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
	sampler2D texture_normal1;
	sampler2D texture_depth1;

	float height_scale;
}; 
  
uniform Material material;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 texCoords)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoords));
    return (ambient + diffuse); // TODO: Fix specular
}  

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse); // TODO: Fix specular
} 

struct SpotLight {
    vec3 direction;
    vec3 position;
	float cutOff;
	float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords)
{
    vec3 lightDir = normalize(-light.direction);
	float theta = dot(lightDir, normalize(-light.direction));
    
    if(theta > light.cutOff) 
    {       
	   float epsilon   = light.cutOff - light.outerCutOff;
       float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);
        // specular shading
        vec3 halfwayDir = normalize(lightDir + viewDir);  
		float spec = pow(max(1 - dot(normal, halfwayDir), 0.0), 16.0);
        // combine results
        vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, texCoords));
        vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, texCoords));
        vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoords));
        return (ambient + diffuse) * intensity; // TODO: Fix specular
    }
    else  // else, use ambient light so scene isn't completely dark outside the spotlight.
	{
        return light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
    }
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float height =  texture(material.texture_depth1, texCoords).r;    
    vec2 p = viewDir.xy / viewDir.z * (height * material.height_scale);
    return texCoords - p;    
} 

void main()
{
    // properties
    //vec3 normal = normalize(Normal);

	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec2 texCoords = ParallaxMapping(TexCoords, viewDir);
	//vec2 texCoords = TexCoords;
	
	// You can still see some weird border artifacts at the edge of the parallax mapped plane. 
	// This happens because at the edges of the plane, the displaced texture coordinates could 
	// oversample outside the range [0, 1] and this gives unrealistic results based on the texture's wrapping mode(s). 
	// A cool trick to solve this issue is to discard the fragment whenever it samples outside the default
	// texture coordinate range:
	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

	vec3 normal = texture(material.texture_normal1, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    //normal = normalize(TBN * normal); 

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, normal, viewDir, texCoords);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
       result += CalcPointLight(pointLights[i], normal, FragPos, viewDir, texCoords);    
    // phase 3: Spot light
    result += CalcSpotLight(spotLight, normal, FragPos, viewDir, texCoords);    
    
    FragColor = vec4(result, 1.0);
}