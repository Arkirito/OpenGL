#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 3) out vec4 gNormal;
layout (location = 2) out vec4 gPosition;
layout (location = 4) out vec4 gAlbedoSpec;

in vec2 TexCoords;

in vec3 FragPos;  
in vec3 Normal; 
in mat3 TBN;

in vec3 TangentViewPos;
in vec3 TangentFragPos;

in vec3 outPos;


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
    vec3 lightDir = normalize(TBN * light.position - fragPos);
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

vec2 SteppedParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * material.height_scale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(material.texture_depth1, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(material.texture_depth1, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    return currentTexCoords;
}

vec2 ParallaxOcclusionMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * material.height_scale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(material.texture_depth1, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(material.texture_depth1, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(material.texture_depth1, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{
    // properties
    //vec3 normal = normalize(Normal);

	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec2 texCoords = ParallaxOcclusionMapping(TexCoords, viewDir);
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
       result += CalcPointLight(pointLights[i], normal, TangentFragPos, viewDir, texCoords);    
    // phase 3: Spot light
    result += CalcSpotLight(spotLight, normal, TangentFragPos, viewDir, texCoords);    
    
    FragColor = vec4(result, 1.0); // forward lightning result

	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 10.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

	    // store the fragment position vector in the first gbuffer texture
    //gPosition = TangentFragPos;
	// gPosition = vec4(texture(material.texture_diffuse1, texCoords).rgb1, 1.0);
    // also store the per-fragment normals into the gbuffer
    // gNormal = vec4(normalize(normal), 1.0);
    // and the diffuse per-fragment color
    //gAlbedoSpec.rgb = texture(material.texture_diffuse1, texCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    //gAlbedoSpec.a = texture(material.texture_specular1, texCoords).r;

	gAlbedoSpec = vec4(texture(material.texture_diffuse1, texCoords).rgb, 1.0);
	gPosition = vec4(FragPos, 1.0);

	gNormal = vec4(Normal, 1.0);
}