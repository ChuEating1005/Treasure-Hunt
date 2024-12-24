#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform bool isDead;

void main() {
    vec4 texColor = texture(texture1, TexCoord);
    if (isDead) {
        texColor = vec4(mix(texColor.rgb, vec3(1.0, 0.0, 0.0), 0.5), texColor.a);
    }
    FragColor = texColor;
}