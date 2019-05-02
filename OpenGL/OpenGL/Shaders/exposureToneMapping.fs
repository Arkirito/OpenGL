#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float exposure;

void main()
{ 
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;

	// Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

	FragColor = vec4(mapped, 1.0);
}