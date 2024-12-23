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
    void toggleScaleAndShimmer() {
        isScalingAndShimmering = !isScalingAndShimmering;
    }
    void toggleWalking() {
        isWalking = !isWalking;
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
    float headBobAngle; 
    float headRotationY;  // Add this line for head rotation
    bool isScalingAndShimmering;
    float scaleTime;
    bool isWalking = false;
};

#endif