#version 330 core
in vec2 TexCoord;
in vec3 normal;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform bool isShimmering;
uniform float shimmerTime;
uniform vec3 rainbowColor;

void main() {
    vec4 texColor = texture(ourTexture, TexCoord);
    
    if (isShimmering) {
        float shimmerIntensity = 0.5 * (sin(shimmerTime * 5.0) * 0.5 + 0.5);
        FragColor = texColor + vec4(shimmerIntensity, shimmerIntensity, shimmerIntensity, 0.0);
    } else {
        FragColor = texColor;
    }
}