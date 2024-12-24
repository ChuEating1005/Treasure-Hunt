#ifndef CHEST_H
#define CHEST_H

#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include "object.h"
#include "shader.h"

using namespace std;

class Chest{
public:
    Chest();
    ~Chest();
    void setup(const string& objDir, const string& textureDir);
    void render(shader_program_t* shader, const glm::mat4& view, const glm::mat4& projection);
    void open();
    void close();
    void update();
    glm::vec3 getPosition() const {
        return container.position;  // Return the container position as Chest's position
    }
private:
    struct ModelPart{
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
        glm::mat4 model;
        Object* object;
    };
    ModelPart lid;
    ModelPart container;

    float scaleRatio;
    float animationTime;
    float openSpeed;
    bool isOpen;
    bool isClose;
    const float PI = 3.14159265359f;
};

#endif // CHEST_H