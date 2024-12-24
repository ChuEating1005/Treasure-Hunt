#ifndef GRASS_H
#define GRASS_H

#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include "object.h"
#include "shader.h"

using namespace std;

class Grass{
public:
    Grass();
    ~Grass();
    void setup(const string& objDir, const string& textureDir);
    void render(shader_program_t* shader, const glm::mat4& view, const glm::mat4& projection);
    void update();
private:
    struct ModelPart{
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
        glm::mat4 model;
        Object* object;
    };
    ModelPart grass;

    float scaleRatio;
};

#endif // GRASS_H