#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Reflection;
in vec3 Refraction;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;

// Empirical parameters from the image
const float SCALE = 0.7;
const float POWER = 2.0;
const float BIAS = 0.2;

void main()
{
    vec3 viewDir = normalize(FragPos - cameraPos);
    float cosTheta = max(dot(-viewDir, Normal), 0.0);
    
    // Calculate empirical factor
    float empiricalFactor = SCALE * pow(cosTheta, POWER) + BIAS;
    empiricalFactor = clamp(empiricalFactor, 0.0, 1.0);
    
    // Sample both reflection and refraction colors
    vec3 reflectColor = texture(skybox, Reflection).rgb;
    vec3 refractColor = texture(skybox, Refraction).rgb;
    
    // Mix colors using empirical approximation
    vec3 finalColor = mix(refractColor, reflectColor, empiricalFactor);
    
    FragColor = vec4(finalColor, 1.0);
}