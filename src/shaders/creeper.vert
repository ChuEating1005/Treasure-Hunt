#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out float ExplodeFactor;
out float AshNoise;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float explodeFactor;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    ExplodeFactor = explodeFactor;
    // Add noise value for ash effect
    AshNoise = fract(sin(dot(aPos.xy, vec2(12.9898, 78.233))) * 43758.5453);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}