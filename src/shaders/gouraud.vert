#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Output the calculated color to be interpolated
out vec3 vertexColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 ambientColor;
uniform vec3 ambientLight;
uniform vec3 diffuseColor;
uniform vec3 diffuseLight;
uniform vec3 specularColor;
uniform vec3 specularLight;
uniform float gloss;
uniform vec3 cameraPos;

void main()
{
    // Transform vertex position to world space
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform normal to world space
    vec3 normVec = mat3(transpose(inverse(model))) * aNormal;
    normVec = normalize(normVec);

    // Ambient
    vec3 ambient = ambientLight * ambientColor;

    // Diffuse
    vec3 lightVec = normalize(lightPos - FragPos);
    float diff = max(dot(normVec, lightVec), 0.0);
    vec3 diffuse = diffuseLight * diffuseColor * diff;

    // Specular
    vec3 viewVec = normalize(cameraPos - FragPos);
    vec3 reflectionVec = reflect(-lightVec, normVec);
    float spec = pow(max(dot(viewVec, reflectionVec), 0.0), gloss);
    vec3 specular = specularLight * specularColor * spec;

    // Calculate final color at vertex
    vertexColor = ambient + diffuse + specular;
    
    // Set final position
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Pass texture coordinates to fragment shader
    TexCoord = aTexCoord;
}