#include "header/creeper.h"
#include <glm/gtc/matrix_transform.hpp>

Creeper::Creeper() : 
    legRotationAngle(0.0f), 
    legRotationSpeed(3.0f),
    bodyHeight(0.0f),
    headBobAngle(0.0f),
    headRotationY(0.0f),
    isScalingAndShimmering(false),
    isWalking(false),
    scaleTime(0.0f)
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
    glm::vec3 basePosition(0.0f, 0.0f, 0.0f);
    glm::vec3 baseScale(8.0f, 8.0f, 8.0f);
    glm::vec3 baseRotation(0.0f, 0.0f, 0.0f);

    // Setup body
    body.position = basePosition;
    body.scale = baseScale;
    body.rotation = baseRotation;
    body.object = new Object(objDir + "creeper/body.obj");
    body.object->load_to_buffer();
    body.object->load_texture(textureDir + "creeper.png");
    body.object->set_texture_parameters();

    // Setup head
    head.position = basePosition;
    head.scale = baseScale;
    head.rotation = baseRotation;
    head.object = new Object(objDir + "creeper/head.obj");
    head.object->load_to_buffer();
    head.object->load_texture(textureDir + "creeper.png");
    head.object->set_texture_parameters();

    leftFrontLeg.position = basePosition + glm::vec3(0.0f, 0.4f, 0.0f);
    leftFrontLeg.scale = baseScale;
    leftFrontLeg.rotation = baseRotation;
    leftFrontLeg.object = new Object(objDir + "creeper/left_front_leg.obj");
    leftFrontLeg.object->load_to_buffer();
    leftFrontLeg.object->load_texture(textureDir + "creeper.png");
    leftFrontLeg.object->set_texture_parameters();

    leftBackLeg.position = basePosition + glm::vec3(0.0f, 0.4f, 0.0f);
    leftBackLeg.scale = baseScale;
    leftBackLeg.rotation = baseRotation;
    leftBackLeg.object = new Object(objDir + "creeper/left_back_leg.obj");
    leftBackLeg.object->load_to_buffer();
    leftBackLeg.object->load_texture(textureDir + "creeper.png");
    leftBackLeg.object->set_texture_parameters();

    rightFrontLeg.position = basePosition + glm::vec3(0.0f, 0.4f, 0.0f);
    rightFrontLeg.scale = baseScale;
    rightFrontLeg.rotation = baseRotation;
    rightFrontLeg.object = new Object(objDir + "creeper/right_front_leg.obj");
    rightFrontLeg.object->load_to_buffer();
    rightFrontLeg.object->load_texture(textureDir + "creeper.png");
    rightFrontLeg.object->set_texture_parameters();

    rightBackLeg.position = basePosition + glm::vec3(0.0f, 0.4f, 0.0f);
    rightBackLeg.scale = baseScale;
    rightBackLeg.rotation = baseRotation;
    rightBackLeg.object = new Object(objDir + "creeper/right_back_leg.obj");
    rightBackLeg.object->load_to_buffer();
    rightBackLeg.object->load_texture(textureDir + "creeper.png");
    rightBackLeg.object->set_texture_parameters();
    
}

void Creeper::update() {
    if (isScalingAndShimmering) {
        scaleTime += 0.024f; // Increased from 0.016f for faster scaling
        float baseScaleFactor = 1.0f + 0.3f * scaleTime; // Increased from 0.2f for faster base scaling
        baseScaleFactor = glm::min(baseScaleFactor, 1.5f);

        float horizontalExtra = 0.0f;
        if (baseScaleFactor > 1.2f) {
            float t = (baseScaleFactor - 1.2f) / 0.3f;
            float smoothT = t * t * (3.0f - 2.0f * t);
            horizontalExtra = 0.4f * smoothT; // Increased from 0.3f for faster horizontal scaling
        }

        // Apply different scales for horizontal and vertical with smooth transition
        glm::vec3 scaleFactor(
            8.0f * (baseScaleFactor + horizontalExtra),
            8.0f * baseScaleFactor,
            8.0f * (baseScaleFactor + horizontalExtra)
        );

        body.scale = scaleFactor;
        head.scale = scaleFactor;
        leftFrontLeg.scale = scaleFactor;
        rightFrontLeg.scale = scaleFactor;
        leftBackLeg.scale = scaleFactor;
        rightBackLeg.scale = scaleFactor;
    } else {
        scaleTime = 0.0f;
        glm::vec3 baseScale(8.0f);
        body.scale = baseScale;
        head.scale = baseScale;
        leftFrontLeg.scale = baseScale;
        rightFrontLeg.scale = baseScale;
        leftBackLeg.scale = baseScale;
        rightBackLeg.scale = baseScale;
    }

    if (isWalking) {
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

    // Apply body bounce
    body.model = glm::mat4(1.0f);
    body.model = glm::translate(body.model, body.position + glm::vec3(0.0f, bodyHeight, 0.0f));
    body.model = glm::scale(body.model, body.scale);
    
    // Apply head bob and rotation
    head.model = glm::mat4(1.0f);
    head.model = glm::translate(head.model, head.position + glm::vec3(0.0f, bodyHeight, 0.0f));
    head.model = glm::rotate(head.model, glm::radians(headRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    head.model = glm::scale(head.model, head.scale);
    
    // Front legs
    leftFrontLeg.model = glm::mat4(1.0f);
    leftFrontLeg.model = glm::translate(leftFrontLeg.model, leftFrontLeg.position + glm::vec3(0.0f, bodyHeight, 0.0f) );
    leftFrontLeg.model = glm::translate(leftFrontLeg.model, glm::vec3(0.0f, frontAttachY, frontAttachZ));
    leftFrontLeg.model = glm::rotate(leftFrontLeg.model, glm::radians(frontLeftLegAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    leftFrontLeg.model = glm::translate(leftFrontLeg.model, glm::vec3(0.0f, -frontAttachY, -frontAttachZ));
    leftFrontLeg.model = glm::scale(leftFrontLeg.model, leftFrontLeg.scale);
    
    rightFrontLeg.model = glm::mat4(1.0f);
    rightFrontLeg.model = glm::translate(rightFrontLeg.model, rightFrontLeg.position + glm::vec3(0.0f, bodyHeight, 0.0f));
    rightFrontLeg.model = glm::translate(rightFrontLeg.model, glm::vec3(0.0f, frontAttachY, frontAttachZ));
    rightFrontLeg.model = glm::rotate(rightFrontLeg.model, glm::radians(frontRightLegAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    rightFrontLeg.model = glm::translate(rightFrontLeg.model, glm::vec3(0.0f, -frontAttachY, -frontAttachZ));
    rightFrontLeg.model = glm::scale(rightFrontLeg.model, rightFrontLeg.scale);
    
    // Back legs
    leftBackLeg.model = glm::mat4(1.0f);
    leftBackLeg.model = glm::translate(leftBackLeg.model, leftBackLeg.position + glm::vec3(0.0f, bodyHeight, 0.0f));
    leftBackLeg.model = glm::translate(leftBackLeg.model, glm::vec3(0.0f, backAttachY, backAttachZ));
    leftBackLeg.model = glm::rotate(leftBackLeg.model, glm::radians(backLeftLegAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    leftBackLeg.model = glm::translate(leftBackLeg.model, glm::vec3(0.0f, -backAttachY, -backAttachZ));
    leftBackLeg.model = glm::scale(leftBackLeg.model, leftBackLeg.scale);
    
    rightBackLeg.model = glm::mat4(1.0f);
    rightBackLeg.model = glm::translate(rightBackLeg.model, rightBackLeg.position + glm::vec3(0.0f, bodyHeight, 0.0f) );
    rightBackLeg.model = glm::translate(rightBackLeg.model, glm::vec3(0.0f, backAttachY, backAttachZ));
    rightBackLeg.model = glm::rotate(rightBackLeg.model, glm::radians(backRightLegAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    rightBackLeg.model = glm::translate(rightBackLeg.model, glm::vec3(0.0f, -backAttachY, -backAttachZ));
    rightBackLeg.model = glm::scale(rightBackLeg.model, rightBackLeg.scale);
}

void Creeper::render(shader_program_t* shader, const glm::mat4& view, const glm::mat4& projection) {
    shader->use();
    
    shader->set_uniform_value("view", view);
    shader->set_uniform_value("projection", projection);

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

    shader->set_uniform_value("isShimmering", isScalingAndShimmering);
    shader->set_uniform_value("shimmerTime", scaleTime);

    shader->release();
}