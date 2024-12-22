#ifndef STEVE_H
#define STEVE_H

#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include "object.h"
#include "shader.h"

using namespace std;

class Steve{
public:
    Steve();
    ~Steve();
    void setup(const string& objDir, const string& textureDir);
    void render(shader_program_t* shader, const glm::mat4& view, const glm::mat4& projection);
    void walk();
    void update();
private:
    struct ModelPart{
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
        glm::mat4 model;
        Object* object;
    };
    ModelPart body;
    ModelPart head;
    ModelPart leftHand;
    ModelPart rightHand;
    ModelPart leftLeg;
    ModelPart rightLeg;

    float animationTime;
    float walkSpeed;
    bool isWalking;
    const float PI = 3.14159265359f;
};

#endif // STEVE_H