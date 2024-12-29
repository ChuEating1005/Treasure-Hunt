#include "header/creeper.h"
#include <glm/gtc/matrix_transform.hpp>

Creeper::Creeper() : 
    legRotationAngle(0.0f), 
    legRotationSpeed(3.0f),
    bodyHeight(0.0f),
    headRotationY(0.0f),
    isScalingAndShimmering(false),
    isWalking(false),
    scaleTime(0.0f),
    explodeFactor(0.0f),
    isExploded(false), // Initialize isExploded
    scaleRatio(8.0f),
    walkingSpeed(0.05f),
    walkingDirection(0.0f, 0.0f, 1.0f),  // Initially walking forward
    isJumping(false),
    verticalVelocity(0.0f),
    initialJumpVelocity(0.5f),  // Increased from 1.0f
    gravity(0.03f),
    groundLevel(0.0f),
    position(0.0f, 0.0f, -50.0f)  // Initialize position
{}

Creeper::~Creeper() {
    delete body.object;
    delete head.object;
    delete leftFrontLeg.object;
    delete rightFrontLeg.object;
    delete leftBackLeg.object;
    delete rightBackLeg.object;
}

void Creeper::setup(const std::string& objDir, const std::string& textureDir) {
    // Initialize base properties
    glm::vec3 baseScale(scaleRatio*1.0f, scaleRatio*1.0f, scaleRatio*1.0f);
    glm::vec3 baseRotation(0.0f, 180.0f, 0.0f);

    // Setup body
    body.position = position;
    body.scale = baseScale;
    body.rotation = baseRotation;
    body.object = new Object(objDir + "creeper/body.obj");
    body.object->load_to_buffer();
    body.object->load_texture(textureDir + "creeper.png");
    body.object->set_texture_parameters();

    // Setup head
    head.position = position;
    head.scale = baseScale;
    head.rotation = baseRotation;
    head.object = new Object(objDir + "creeper/head.obj");
    head.object->load_to_buffer();
    head.object->load_texture(textureDir + "creeper.png");
    head.object->set_texture_parameters();

    // Setup front legs
    leftFrontLeg.position = position + glm::vec3(0.0f, 0.4f, 0.0f);
    leftFrontLeg.scale = baseScale;
    leftFrontLeg.rotation = baseRotation;
    leftFrontLeg.object = new Object(objDir + "creeper/left_front_leg.obj");
    leftFrontLeg.object->load_to_buffer();
    leftFrontLeg.object->load_texture(textureDir + "creeper.png");
    leftFrontLeg.object->set_texture_parameters();

    rightFrontLeg.position = position + glm::vec3(0.0f, 0.4f, 0.0f);
    rightFrontLeg.scale = baseScale;
    rightFrontLeg.rotation = baseRotation;
    rightFrontLeg.object = new Object(objDir + "creeper/right_front_leg.obj");
    rightFrontLeg.object->load_to_buffer();
    rightFrontLeg.object->load_texture(textureDir + "creeper.png");
    rightFrontLeg.object->set_texture_parameters();

    // Setup back legs
    leftBackLeg.position = position + glm::vec3(0.0f, 0.4f, 0.0f);
    leftBackLeg.scale = baseScale;
    leftBackLeg.rotation = baseRotation;
    leftBackLeg.object = new Object(objDir + "creeper/left_back_leg.obj");
    leftBackLeg.object->load_to_buffer();
    leftBackLeg.object->load_texture(textureDir + "creeper.png");
    leftBackLeg.object->set_texture_parameters();

    rightBackLeg.position = position + glm::vec3(0.0f, 0.4f, 0.0f);
    rightBackLeg.scale = baseScale;
    rightBackLeg.rotation = baseRotation;
    rightBackLeg.object = new Object(objDir + "creeper/right_back_leg.obj");
    rightBackLeg.object->load_to_buffer();
    rightBackLeg.object->load_texture(textureDir + "creeper.png");
    rightBackLeg.object->set_texture_parameters();
    
}

// Modify toggleScaleAndShimmer to initiate scaling and trigger explosion after scaling
void Creeper::toggleScaleAndShimmer() {
    if (!isExploded && !isScalingAndShimmering) {
        isScalingAndShimmering = true;
    } else if (isExploded) {
        // Reset explosion
        isExploded = false;
        explodeFactor = 0.0f;
        // Reset scale
        glm::vec3 baseScale(8.0f);
        body.scale = baseScale;
        head.scale = baseScale;
        leftFrontLeg.scale = baseScale;
        rightFrontLeg.scale = baseScale;
        leftBackLeg.scale = baseScale;
        rightBackLeg.scale = baseScale;
    }
}

// Modify update to handle scaling and transition to explosion
void Creeper::update() {
    // Handle jumping physics (independent of walking)
    if (isJumping) {
        // Apply gravity and update position
        verticalVelocity -= gravity;
        bodyHeight += verticalVelocity;
        
        // Continue moving in the jumping direction if we were walking when we jumped
        if (isWalking && glm::length(walkingDirection) > 0.0f) {
            position += walkingDirection * walkingSpeed * 1.2f;  // Slightly faster movement while jumping
        }
        
        // Check if we've hit the ground level (which might be the top of the chest)
        if (bodyHeight <= groundLevel) {
            bodyHeight = groundLevel;
            verticalVelocity = 0.0f;
            isJumping = false;
        }
    }

    // Handle walking (can happen simultaneously with jumping)
    if (isWalking && glm::length(walkingDirection) > 0.0f) {
        // Update position based on walking direction
        position += walkingDirection * walkingSpeed;
        
        // Update leg animation
        legRotationAngle += legRotationSpeed;
        if (legRotationAngle >= 360.0f) {
            legRotationAngle = 0.0f;
        }
    }

    // Update all parts positions
    glm::vec3 currentPos = position + glm::vec3(0.0f, bodyHeight, 0.0f);
    body.position = currentPos;
    head.position = currentPos;
    leftFrontLeg.position = currentPos;
    rightFrontLeg.position = currentPos;
    leftBackLeg.position = currentPos;
    rightBackLeg.position = currentPos;

    if (isExploded) {
        // Gradually increase the explosion factor
        explodeFactor += 0.05f;
        if (explodeFactor > 1.0f) {
            explodeFactor = 1.0f;
        }
    } else {
        // Handle jumping physics
        if (isJumping) {
            // Apply gravity and update position
            verticalVelocity -= gravity;
            bodyHeight += verticalVelocity;
            
            // Check if we've hit the ground
            if (bodyHeight <= groundLevel) {
                bodyHeight = groundLevel;
                verticalVelocity = 0.0f;
                isJumping = false;
            }
        }

        if (isScalingAndShimmering) {
            scaleTime += 0.024f;
            float scaleFactor = 1.0f + 0.3f * scaleTime;
            
            // If we've reached max scale, start the explosion
            if (scaleFactor >= 1.5f) {
                isScalingAndShimmering = false;
                isExploded = true;
                scaleTime = 0.0f;
            } else {
                scaleFactor = glm::min(scaleFactor, 1.5f);

                // Scale more in width (x,z) and less in height (y)
                float heightScale = 1.0f + 0.15f * scaleTime; // Half the scale factor for height
                heightScale = glm::min(heightScale, 1.25f);
                
                glm::vec3 baseScale(8.0f * scaleFactor, 8.0f * heightScale, 8.0f * scaleFactor);

                body.scale = baseScale;
                head.scale = baseScale;
                leftFrontLeg.scale = baseScale;
                rightFrontLeg.scale = baseScale;
                leftBackLeg.scale = baseScale;
                rightBackLeg.scale = baseScale;
            }
        } else {
            if (explodeFactor == 0.0f) {  // Only reset scale if not exploding
                scaleTime = 0.0f;
                glm::vec3 baseScale(8.0f);
                body.scale = baseScale;
                head.scale = baseScale;
                leftFrontLeg.scale = baseScale;
                rightFrontLeg.scale = baseScale;
                leftBackLeg.scale = baseScale;
                rightBackLeg.scale = baseScale;
            }
        }

        if (isWalking) {
            // Update position based on walking direction
            position += walkingDirection * walkingSpeed;
            
            // Update all parts positions
            body.position = position;
            head.position = position;
            leftFrontLeg.position = position;
            rightFrontLeg.position = position;
            leftBackLeg.position = position;
            rightBackLeg.position = position;

            // Update leg animation
            legRotationAngle += legRotationSpeed;
            if (legRotationAngle >= 360.0f) {
                legRotationAngle = 0.0f;
            }
            
            // Update body height with a subtle bounce
            bodyHeight = 0.2f * sin(glm::radians(legRotationAngle * 2.0f));
        }
        
        // Calculate diagonal leg movements (opposite legs move together)
        float frontLeftLegAngle = 10.0f * sin(glm::radians(legRotationAngle));
        float frontRightLegAngle = -frontLeftLegAngle;
        float backLeftLegAngle = -frontLeftLegAngle;
        float backRightLegAngle = frontLeftLegAngle;

        // Constants for attachment points
        const float frontAttachY = 3.0f;
        const float backAttachY = 3.0f;
        const float frontAttachZ = 3.0f;
        const float backAttachZ = -3.0f;

        // Apply body bounce and rotation
        body.model = glm::mat4(1.0f);
        body.model = glm::translate(body.model, body.position + glm::vec3(0.0f, bodyHeight, 0.0f));
        body.model = glm::rotate(body.model, glm::radians(body.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        body.model = glm::scale(body.model, body.scale);
        
        // Apply head bob and rotation
        head.model = glm::mat4(1.0f);
        head.model = glm::translate(head.model, head.position + glm::vec3(0.0f, bodyHeight, 0.0f));
        head.model = glm::rotate(head.model, glm::radians(head.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Apply body rotation
        head.model = glm::rotate(head.model, glm::radians(headRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        head.model = glm::scale(head.model, head.scale);
        
        // Front legs (apply body rotation to legs as well)
        leftFrontLeg.model = glm::mat4(1.0f);
        leftFrontLeg.model = glm::translate(leftFrontLeg.model, leftFrontLeg.position + glm::vec3(0.0f, bodyHeight, 0.0f));
        leftFrontLeg.model = glm::rotate(leftFrontLeg.model, glm::radians(leftFrontLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        leftFrontLeg.model = glm::translate(leftFrontLeg.model, glm::vec3(0.0f, frontAttachY, frontAttachZ));
        leftFrontLeg.model = glm::rotate(leftFrontLeg.model, glm::radians(frontLeftLegAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        leftFrontLeg.model = glm::translate(leftFrontLeg.model, glm::vec3(0.0f, -frontAttachY, -frontAttachZ));
        leftFrontLeg.model = glm::scale(leftFrontLeg.model, leftFrontLeg.scale);
        
        rightFrontLeg.model = glm::mat4(1.0f);
        rightFrontLeg.model = glm::translate(rightFrontLeg.model, rightFrontLeg.position + glm::vec3(0.0f, bodyHeight, 0.0f));
        rightFrontLeg.model = glm::rotate(rightFrontLeg.model, glm::radians(rightFrontLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        rightFrontLeg.model = glm::translate(rightFrontLeg.model, glm::vec3(0.0f, frontAttachY, frontAttachZ));
        rightFrontLeg.model = glm::rotate(rightFrontLeg.model, glm::radians(frontRightLegAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        rightFrontLeg.model = glm::translate(rightFrontLeg.model, glm::vec3(0.0f, -frontAttachY, -frontAttachZ));
        rightFrontLeg.model = glm::scale(rightFrontLeg.model, rightFrontLeg.scale);
        
        // Back legs (apply body rotation to legs as well)
        leftBackLeg.model = glm::mat4(1.0f);
        leftBackLeg.model = glm::translate(leftBackLeg.model, leftBackLeg.position + glm::vec3(0.0f, bodyHeight, 0.0f));
        leftBackLeg.model = glm::rotate(leftBackLeg.model, glm::radians(leftBackLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        leftBackLeg.model = glm::translate(leftBackLeg.model, glm::vec3(0.0f, backAttachY, backAttachZ));
        leftBackLeg.model = glm::rotate(leftBackLeg.model, glm::radians(backLeftLegAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        leftBackLeg.model = glm::translate(leftBackLeg.model, glm::vec3(0.0f, -backAttachY, -backAttachZ));
        leftBackLeg.model = glm::scale(leftBackLeg.model, leftBackLeg.scale);
        
        rightBackLeg.model = glm::mat4(1.0f);
        rightBackLeg.model = glm::translate(rightBackLeg.model, rightBackLeg.position + glm::vec3(0.0f, bodyHeight, 0.0f) );
        rightBackLeg.model = glm::rotate(rightBackLeg.model, glm::radians(rightBackLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        rightBackLeg.model = glm::translate(rightBackLeg.model, glm::vec3(0.0f, backAttachY, backAttachZ));
        rightBackLeg.model = glm::rotate(rightBackLeg.model, glm::radians(backRightLegAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        rightBackLeg.model = glm::translate(rightBackLeg.model, glm::vec3(0.0f, -backAttachY, -backAttachZ));
        rightBackLeg.model = glm::scale(rightBackLeg.model, rightBackLeg.scale);

        if (isExploded) {
            // Gradually increase the explosion factor
            explodeFactor += 0.05f;
            if (explodeFactor > 1.0f) {
                explodeFactor = 1.0f;
            }
        } else {
            if (explodeFactor > 0.0f) {
                explodeFactor -= 0.02f; // Slowed down the explosion effect
                if (explodeFactor < 0.0f) {
                    explodeFactor = 0.0f;
                }
            }
        }
    }
}

void Creeper::render(shader_program_t* shader, const glm::mat4& view, const glm::mat4& projection) {
    shader->use();
    
    shader->set_uniform_value("view", view);
    shader->set_uniform_value("projection", projection);

    // Set material properties
    shader->set_uniform_value("isShimmering", isScalingAndShimmering);
    shader->set_uniform_value("shimmerTime", scaleTime);
    shader->set_uniform_value("explodeFactor", explodeFactor);

    // Add white flash uniform
    bool whiteFlash = isExploded && (explodeFactor > 0.0f);
    shader->set_uniform_value("whiteFlash", whiteFlash);

    // Render body
    shader->set_uniform_value("model", body.model);
    body.object->render();

    // Render head
    shader->set_uniform_value("model", head.model);
    head.object->render();

    // Render legs
    shader->set_uniform_value("model", leftFrontLeg.model);
    leftFrontLeg.object->render();
    shader->set_uniform_value("model", rightFrontLeg.model);
    rightFrontLeg.object->render();
    shader->set_uniform_value("model", leftBackLeg.model);
    leftBackLeg.object->render();
    shader->set_uniform_value("model", rightBackLeg.model);
    rightBackLeg.object->render();

    shader->release();
}