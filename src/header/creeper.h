#ifndef CREEPER_H
#define CREEPER_H

#include <glm/glm.hpp>
#include "object.h"
#include "shader.h"

class Creeper {
public:
    Creeper();
    ~Creeper();
    
    void setup(const std::string& objDir, const std::string& textureDir);
    void update();
    void render(shader_program_t* shader, const glm::mat4& view, const glm::mat4& projection);
    void rotateHead(float angle) {
        float newRotation = headRotationY + angle;
        if (newRotation >= -60.0f && newRotation <= 60.0f) {
            headRotationY = newRotation;
        }
    }
    void toggleScaleAndShimmer();
    void toggleWalking() {
        isWalking = !isWalking;
    }
    void triggerExplosion() {
        explodeFactor = 1.0f;
    }

private:
    struct ModelPart {
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
        Object* object;
        glm::mat4 model;
    };

    ModelPart body;
    ModelPart head;
    ModelPart leftFrontLeg;
    ModelPart rightFrontLeg;
    ModelPart leftBackLeg;
    ModelPart rightBackLeg;

    float legRotationAngle;
    float legRotationSpeed;
    float bodyHeight; 
    float headRotationY;
    bool isScalingAndShimmering;
    float scaleTime;
    float explodeFactor;
    bool isWalking = false;
    bool isExploded; 
    float scaleRatio;
    float walkingSpeed;
    glm::vec3 walkingDirection;
};

#endif