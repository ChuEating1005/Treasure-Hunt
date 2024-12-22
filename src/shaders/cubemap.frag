#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
    // Sample the cubemap using the texture coordinates
    FragColor = texture(skybox, TexCoords);
}