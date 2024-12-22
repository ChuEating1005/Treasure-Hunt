#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec3 Reflection;
out vec3 Refraction;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

const float AIR_COEFF = 1.0;
const float GLASS_COEFF = 1.52;

vec3 calculateReflection(vec3 I, vec3 N) {
    return I - 2.0 * dot(I, N) * N;
}

vec3 calculateRefraction(vec3 I, vec3 N, float eta) {
    float cosI = dot(-I, N);
    float sinT2 = eta * eta * (1.0 - cosI * cosI);
    if (sinT2 > 1.0) return vec3(0.0); // Total internal reflection
    float cosT = sqrt(1.0 - sinT2);
    return eta * I + (eta * cosI - cosT) * N;
}

void main()
{
    // Transform position and normal
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    
    // Calculate view direction
    vec3 viewDir = normalize(FragPos - cameraPos);
    
    // Calculate reflection and refraction vectors
    Reflection = calculateReflection(viewDir, Normal);
    Refraction = calculateRefraction(viewDir, Normal, AIR_COEFF/GLASS_COEFF);
    
    // Pass texture coordinates
    TexCoord = aTexCoord;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}