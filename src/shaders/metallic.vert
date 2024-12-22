#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec3 Reflection;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

void main() 
{
    // Transform vertex position and normal to world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Calculate reflection vector
    vec3 viewVec = normalize(FragPos - cameraPos);
    Reflection = reflect(viewVec, normalize(Normal));
    
    // Pass texture coordinates
    TexCoord = aTexCoord;
    
    // Set final position
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}