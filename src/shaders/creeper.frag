#version 330 core
in vec2 gTexCoord;
in float gExplodeFactor;
in float gAlpha;
in float gAshIntensity;
in vec3 gExplosionVector;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform bool isShimmering;
uniform float shimmerTime;
uniform vec3 rainbowColor;
uniform bool whiteFlash;

void main() {
    vec4 texColor = texture(ourTexture, gTexCoord);
    
    // Only apply shimmering effect if isShimmering flag is true
    if (whiteFlash && gExplodeFactor > 0.0) {
        texColor = vec4(1.0, 1.0, 1.0, 1.0);
    } else if (isShimmering) {
        float shimmerIntensity = 0.5 * (sin(shimmerTime * 5.0) * 0.5 + 0.5);
        texColor += vec4(shimmerIntensity, shimmerIntensity, shimmerIntensity, 0.0);
    }
    
    // Enhanced explosion effects
    if (gExplodeFactor > 0.0) {
        // Define Minecraft-like explosion colors
        vec3 fireCore = vec3(1.0, 0.7, 0.0);    // Bright yellow core
        vec3 fireOuter = vec3(0.8, 0.3, 0.0);   // Orange outer
        vec3 smoke = vec3(0.6, 0.6, 0.6);       // Light gray smoke
        
        // Create pixelated noise pattern
        float noise = floor(fract(sin(dot(floor(gTexCoord * 32.0), vec2(12.9898, 78.233))) * 43758.5453) * 4.0) / 4.0;
        
        // Mix colors in a more discrete way
        vec3 explosionColor = mix(
            mix(fireCore, fireOuter, step(0.5, noise)),
            smoke,
            step(0.7, gExplodeFactor)
        );
        
        // Add pixelated variation
        float pixelatedAsh = floor(gAshIntensity * 8.0) / 8.0;
        explosionColor *= (1.0 + pixelatedAsh * 0.3);
        
        // Create blocky flame effect
        float blockFlame = step(0.5, sin(gTexCoord.y * 16.0 + gExplodeFactor * 8.0));
        explosionColor *= (1.0 + blockFlame * 0.2);
        
        texColor.rgb = explosionColor;
        texColor.a *= (1.0 - pow(gExplodeFactor, 2.0));
        
        // Add pixelated highlights
        float highlight = step(0.8, noise) * (1.0 - gExplodeFactor);
        texColor.rgb += vec3(highlight);
    }
    
    // Make alpha transitions more discrete
    float minAlpha = gExplodeFactor > 0.0 ? 0.6 : 0.0;
    texColor.a = floor(max(texColor.a, minAlpha) * 4.0) / 4.0;
    
    FragColor = texColor;
}