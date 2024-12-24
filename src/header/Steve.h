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
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void stopMoving();
    void rotateHead(float xoffset, float yoffset);
    void die();
    void revive();
    bool getIsDead() const { return isDead; }
    glm::vec3 getEyePosition() const;
    glm::vec3 getViewDirection() const;
    float getHeadRotationX() const { return headRotationX; }
    float getHeadRotationY() const { return headRotationY; }
    
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

    glm::vec3 direction;
    float animationTime;
    float moveSpeed;
    float swingSpeed;
    float headRotationX;
    float headRotationY;
    float deathRotation;
    bool isWalking;
    bool isDead;

    const float PI = 3.14159265359f;
    const float HEAD_ROTATION_SENSITIVITY = 0.1f;
    const float DEATH_ROTATION_SPEED = 7.0f;
    
    // Offset of each part
    const glm::vec3 headOffset = glm::vec3(0.0f, 6.0f, 0.0f);
    const glm::vec3 lefthandOffset = glm::vec3(-2.44f, 4.88f, 0.0f);
    const glm::vec3 righthandOffset = glm::vec3(2.44f, 4.88f, 0.0f);
    
};

#endif // STEVE_H