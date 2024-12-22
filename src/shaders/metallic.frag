#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Reflection;

out vec4 FragColor;

uniform vec3 lightPos;
uniform sampler2D ourTexture;
uniform samplerCube skybox;

void main() 
{
    // Get base color from texture
    vec4 modelColor = texture(ourTexture, TexCoord);
    
    // Calculate Lambertian reflection (diffuse)
    vec3 norm = normalize(Normal);
    vec3 lightVec = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightVec), 0.0);
    
    // Add bias to diffuse (as shown in the formula B = Bd + bias)
    float bias = 0.2;
    float B = diff + bias;
    
    // Sample environment reflection color from skybox
    vec3 reflectColor = vec3(texture(skybox, Reflection));
    
    // Mix the colors using the metallic ratio (alpha)
    float alpha = 0.4; // Adjust this value to control metallicness
    vec3 finalColor = alpha * B * modelColor.rgb + (1.0 - alpha) * reflectColor;
    
    // Output final color
    FragColor = vec4(finalColor, 1.0);
}	
