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
    void triggerExplosion() {
        explodeFactor = 1.0f;
    }
    void jump() {
        if (!isJumping) {  // Only start a new jump if not already jumping
            isJumping = true;
            verticalVelocity = initialJumpVelocity;
            // Store the direction if we're currently walking
            if (isWalking) {
                jumpingDirection = walkingDirection;
            }
        }
    }
    
    glm::vec3 getPosition() const {
        return position;
    }
    
    glm::vec3 getNextPosition() const {
        // Calculate the next position based on current movement
        return position + getMovementDirection();
    }
    
    void stopMoving() {
        isWalking = false;
        walkingDirection = glm::vec3(0.0f);
        // Note: We don't stop jumping here to allow independent jump/walk states
    }
    
    void setWalkingDirection(const glm::vec3& direction) {
        walkingDirection = glm::normalize(direction);
    }

    glm::vec3 getMovementDirection() const {
        if (!isWalking) return glm::vec3(0.0f);
        return walkingDirection * walkingSpeed;
    }

    bool getIsJumping() const {
        return isJumping;
    }

    float getYPosition() const {
        return position.y + bodyHeight;
    }

    // Add direction control methods
    void startMovingForward() {
        isWalking = true;
        walkingDirection = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    void startMovingBackward() {
        if (isWalking) {
            walkingDirection = glm::vec3(0.0f, 0.0f, -1.0f);
        }
    }

    void setGroundLevel(float newGroundLevel) {
        groundLevel = newGroundLevel;
    }

    float getGroundLevel() const {
        return groundLevel;
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
    bool isJumping;
    float verticalVelocity;
    float initialJumpVelocity;
    float gravity;
    float groundLevel;
    glm::vec3 position;
    glm::vec3 jumpingDirection; // Add this new member variable
};

#endif