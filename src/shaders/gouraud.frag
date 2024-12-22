#version 330 core

// Receive interpolated color from vertex shader
in vec3 vertexColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    // Use the interpolated color directly
    FragColor = vec4(vertexColor, 1.0) * texture(ourTexture, TexCoord);
}