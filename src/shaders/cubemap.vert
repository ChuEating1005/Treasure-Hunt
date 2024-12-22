#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    // Cubemap texture coordinates are the same as the vertex positions
    TexCoords = aPos;
    
    // Remove translation from view matrix by converting to mat3 and back to mat4
    mat4 viewNoTranslation = mat4(mat3(view));
    
    // Note: The position is multiplied by the projection and view matrix
    // We set w to 1.0 to ensure the skybox stays in the background
    vec4 pos = projection * viewNoTranslation * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // ensure skybox is rendered at maximum depth
}