#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Reflection;
in vec3 Refraction;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;

const float AIR_COEFF = 1.0;
const float GLASS_COEFF = 1.52;

void main()
{
    vec3 viewDir = normalize(FragPos - cameraPos);
    
    // Calculate Schlick's approximation
    float ratio = AIR_COEFF / GLASS_COEFF;
    float cosTheta = max(dot(-viewDir, Normal), 0.0);
    float r0 = pow((AIR_COEFF - GLASS_COEFF) / (AIR_COEFF + GLASS_COEFF), 2.0);
    float schlickFactor = r0 + (1.0 - r0) * pow(1.0 - cosTheta, 5.0);
    
    // Sample both reflection and refraction colors
    vec3 reflectColor = texture(skybox, Reflection).rgb;
    vec3 refractColor = texture(skybox, Refraction).rgb;
    
    // Mix colors using Schlick approximation
    vec3 finalColor = mix(refractColor, reflectColor, schlickFactor);
    
    FragColor = vec4(finalColor, 1.0);
}