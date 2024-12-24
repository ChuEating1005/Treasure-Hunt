#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 96) out; // Increased for more triangles

in vec3 FragPos[];
in vec3 Normal[];
in vec2 TexCoord[];
in float ExplodeFactor[];
in float AshNoise[];

out vec2 gTexCoord;
out float gExplodeFactor;
out float gAlpha;
out float gAshIntensity;
out vec3 gExplosionVector;

vec3 quantizeDirection(vec3 dir, float factor) {
    // Quantize to create more blocky movement paths
    vec3 absDir = abs(dir);
    vec3 signs = sign(dir);
    
    // Create more discrete steps (8 possible directions per axis)
    float steps = 8.0;
    vec3 quantized = signs * ceil(absDir * steps) / steps;
    
    // Ensure movements are more axis-aligned
    float maxComp = max(max(absDir.x, absDir.y), absDir.z);
    if (maxComp == absDir.x) quantized.yz *= 0.5;
    else if (maxComp == absDir.y) quantized.xz *= 0.5;
    else quantized.xy *= 0.5;
    
    return normalize(quantized);
}

// Helper function to interpolate attributes
vec2 interpolateTexCoord(vec2 v1, vec2 v2, float t) {
    return v1 + t * (v2 - v1);
}

// Separate function for emitting a single triangle
void emitSingleTriangle(vec4 v1, vec4 v2, vec4 v3, 
                       vec2 t1, vec2 t2, vec2 t3, 
                       float explodeFactor, float ashNoise, float randSeed) {
    // Create more blocky movement
    vec3 uniqueDir = quantizeDirection(normalize(vec3(
        sin(randSeed * 6.28),
        cos(randSeed * 6.28) + 0.5,
        sin(randSeed * 12.56)
    )), 8.0);
    
    // Reduce base size for smaller fragments
    float baseSize = 2.0 + randSeed * 0.75; // Reduced from 2.0 to 1.0
    
    // Reduce offset multiplier for smaller explosion radius
    vec3 offset = uniqueDir * (20.0 * baseSize) * pow(explodeFactor, 2.0); // Reduced from 40.0 to 20.0
    
    // Add slight rotation for more dynamic movement
    float rotAngle = floor(randSeed * 4.0) * 90.0; // Only 90-degree rotations
    
    // Calculate center point for scaling
    vec4 center = (v1 + v2 + v3) / 3.0;
    
    // Make fragments more cubic by scaling them uniformly
    float scale = 0.8 + randSeed * 0.3; // Reduced from 1.0 to 0.8
    
    // Apply transformations
    v1 = center + scale * (v1 - center);
    v2 = center + scale * (v2 - center);
    v3 = center + scale * (v3 - center);
    
    // Emit vertices with transformed positions
    gl_Position = v1;
    gl_Position.xyz += offset;
    gTexCoord = t1;
    gExplodeFactor = explodeFactor;
    gAlpha = 1.0 - pow(explodeFactor, 1.5);
    gAshIntensity = ashNoise + randSeed;
    gExplosionVector = uniqueDir;
    EmitVertex();

    gl_Position = v2;
    gl_Position.xyz += offset;
    gTexCoord = t2;
    gExplodeFactor = explodeFactor;
    gAlpha = 1.0 - pow(explodeFactor, 1.5);
    gAshIntensity = ashNoise + randSeed;
    gExplosionVector = uniqueDir;
    EmitVertex();

    gl_Position = v3;
    gl_Position.xyz += offset;
    gTexCoord = t3;
    gExplodeFactor = explodeFactor;
    gAlpha = 1.0 - pow(explodeFactor, 1.5);
    gAshIntensity = ashNoise + randSeed;
    gExplosionVector = uniqueDir;
    EmitVertex();
    
    EndPrimitive();
}

void emitSubdividedTriangle(vec4 pos1, vec4 pos2, vec4 pos3, 
                           vec2 tex1, vec2 tex2, vec2 tex3,
                           float explodeFactor, float ashNoise) {
    // Calculate center and midpoints
    vec4 center = (pos1 + pos2 + pos3) / 3.0;
    vec2 centerTex = (tex1 + tex2 + tex3) / 3.0;
    
    // Calculate edge points
    vec4[12] edgePoints;
    vec2[12] edgeTexCoords;
    
    // Calculate edge points and texture coordinates
    edgePoints[0] = mix(pos1, pos2, 0.25);
    edgePoints[1] = mix(pos1, pos2, 0.5);
    edgePoints[2] = mix(pos1, pos2, 0.75);
    edgePoints[3] = mix(pos2, pos3, 0.25);
    edgePoints[4] = mix(pos2, pos3, 0.5);
    edgePoints[5] = mix(pos2, pos3, 0.75);
    edgePoints[6] = mix(pos3, pos1, 0.25);
    edgePoints[7] = mix(pos3, pos1, 0.5);
    edgePoints[8] = mix(pos3, pos1, 0.75);
    edgePoints[9] = mix(pos1, center, 0.5);
    edgePoints[10] = mix(pos2, center, 0.5);
    edgePoints[11] = mix(pos3, center, 0.5);
    
    edgeTexCoords[0] = mix(tex1, tex2, 0.25);
    edgeTexCoords[1] = mix(tex1, tex2, 0.5);
    edgeTexCoords[2] = mix(tex1, tex2, 0.75);
    edgeTexCoords[3] = mix(tex2, tex3, 0.25);
    edgeTexCoords[4] = mix(tex2, tex3, 0.5);
    edgeTexCoords[5] = mix(tex2, tex3, 0.75);
    edgeTexCoords[6] = mix(tex3, tex1, 0.25);
    edgeTexCoords[7] = mix(tex3, tex1, 0.5);
    edgeTexCoords[8] = mix(tex3, tex1, 0.75);
    edgeTexCoords[9] = mix(tex1, centerTex, 0.5);
    edgeTexCoords[10] = mix(tex2, centerTex, 0.5);
    edgeTexCoords[11] = mix(tex3, centerTex, 0.5);

    float baseRand = fract(sin(dot(vec2(center.xy), vec2(12.9898, 78.233))) * 43758.5453);
    
    // Emit all triangles
    emitSingleTriangle(pos1, edgePoints[0], edgePoints[8], tex1, edgeTexCoords[0], edgeTexCoords[8], explodeFactor, ashNoise, baseRand);
    emitSingleTriangle(pos2, edgePoints[3], edgePoints[1], tex2, edgeTexCoords[3], edgeTexCoords[1], explodeFactor, ashNoise, baseRand + 0.1);
    emitSingleTriangle(pos3, edgePoints[6], edgePoints[5], tex3, edgeTexCoords[6], edgeTexCoords[5], explodeFactor, ashNoise, baseRand + 0.2);
    
    emitSingleTriangle(edgePoints[0], edgePoints[1], center, edgeTexCoords[0], edgeTexCoords[1], centerTex, explodeFactor, ashNoise, baseRand + 0.3);
    emitSingleTriangle(edgePoints[3], edgePoints[4], center, edgeTexCoords[3], edgeTexCoords[4], centerTex, explodeFactor, ashNoise, baseRand + 0.4);
    emitSingleTriangle(edgePoints[6], edgePoints[7], center, edgeTexCoords[6], edgeTexCoords[7], centerTex, explodeFactor, ashNoise, baseRand + 0.5);
    
    emitSingleTriangle(edgePoints[1], edgePoints[2], center, edgeTexCoords[1], edgeTexCoords[2], centerTex, explodeFactor, ashNoise, baseRand + 0.6);
    emitSingleTriangle(edgePoints[4], edgePoints[5], center, edgeTexCoords[4], edgeTexCoords[5], centerTex, explodeFactor, ashNoise, baseRand + 0.7);
    emitSingleTriangle(edgePoints[7], edgePoints[8], center, edgeTexCoords[7], edgeTexCoords[8], centerTex, explodeFactor, ashNoise, baseRand + 0.8);
    
    emitSingleTriangle(edgePoints[9], edgePoints[10], center, edgeTexCoords[9], edgeTexCoords[10], centerTex, explodeFactor, ashNoise, baseRand + 0.9);
    emitSingleTriangle(edgePoints[10], edgePoints[11], center, edgeTexCoords[10], edgeTexCoords[11], centerTex, explodeFactor, ashNoise, baseRand + 1.0);
    emitSingleTriangle(edgePoints[11], edgePoints[9], center, edgeTexCoords[11], edgeTexCoords[9], centerTex, explodeFactor, ashNoise, baseRand + 1.1);
}

void main() {
    if (ExplodeFactor[0] > 0.0) {
        // For explosion, subdivide the triangle
        emitSubdividedTriangle(
            gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position,
            TexCoord[0], TexCoord[1], TexCoord[2],
            ExplodeFactor[0], AshNoise[0]
        );
    } else {
        // No explosion, emit original triangle
        for(int i = 0; i < 3; i++) {
            gl_Position = gl_in[i].gl_Position;
            gTexCoord = TexCoord[i];
            gExplodeFactor = ExplodeFactor[i];
            gAlpha = 1.0;
            gAshIntensity = AshNoise[i];
            gExplosionVector = vec3(0.0);
            EmitVertex();
        }
        EndPrimitive();
    }
}