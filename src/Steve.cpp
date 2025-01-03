#include "header/Steve.h"
#include <unistd.h> 
#include <bits/stdc++.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Steve::Steve() {
    // Initialize animation variables
    animationTime = 0.0f;
    swingSpeed = 0.05f;
    moveSpeed = 1.0f;
    isWalking = false;
    direction = glm::vec3(0.0f, 0.0f, 1.0f);  // 預設朝向 -z 方向
    headRotationX = 0.0f;
    headRotationY = 0.0f;
    isDead = false;
    deathRotation = 0.0f;
    isSwinging = false;
    swingRotation = 0.0f;
    swingForward = true;
}

Steve::~Steve() {
    delete body.object;
    delete head.object;
    delete leftHand.object;
    delete rightHand.object;
    delete leftLeg.object;
    delete rightLeg.object;
}

void Steve::setup(const string& objDir, const string& textureDir){
    
    body.position = glm::vec3(0.0f, 5.0f, -10.0f);
    body.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    body.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    body.object = new Object(objDir + "steve/body.obj");
    body.object->load_to_buffer();
    body.object->load_texture(textureDir + "steve.png");
    body.model = glm::mat4(1.0f);
    body.model = glm::translate(body.model, body.position);
    body.model = glm::rotate(body.model, body.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    body.model = glm::scale(body.model, body.scale);

    head.position = body.position;
    head.scale = body.scale;
    head.rotation = body.rotation;
    head.object = new Object(objDir + "steve/head.obj");
    head.object->load_to_buffer();
    head.object->load_texture(textureDir + "steve.png");
    head.model = glm::mat4(1.0f);
    head.model = glm::translate(head.model, head.position);
    head.model = glm::rotate(head.model, head.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    head.model = glm::scale(head.model, head.scale);

    leftHand.position = body.position;
    leftHand.scale = body.scale;
    leftHand.rotation = body.rotation;
    leftHand.object = new Object(objDir + "steve/left_handv2.obj");
    leftHand.object->load_to_buffer();
    leftHand.object->load_texture(textureDir + "steve.png");
    leftHand.model = glm::mat4(1.0f);
    leftHand.model = glm::translate(leftHand.model, leftHand.position);
    leftHand.model = glm::rotate(leftHand.model, leftHand.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    leftHand.model = glm::scale(leftHand.model, leftHand.scale);

    rightHand.position = body.position;
    rightHand.scale = body.scale;
    rightHand.rotation = body.rotation;
    rightHand.object = new Object(objDir + "steve/right_handv2.obj");
    rightHand.object->load_to_buffer();
    rightHand.object->load_texture(textureDir + "steve.png");
    rightHand.model = glm::mat4(1.0f);
    rightHand.model = glm::translate(rightHand.model, rightHand.position);
    rightHand.model = glm::rotate(rightHand.model, rightHand.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    rightHand.model = glm::scale(rightHand.model, rightHand.scale);

    leftLeg.position = body.position;
    leftLeg.scale = body.scale;
    leftLeg.rotation = body.rotation;
    leftLeg.object = new Object(objDir + "steve/left_legv2.obj");
    leftLeg.object->load_to_buffer();
    leftLeg.object->load_texture(textureDir + "steve.png");
    leftLeg.model = glm::mat4(1.0f);
    leftLeg.model = glm::translate(leftLeg.model, leftLeg.position);
    leftLeg.model = glm::rotate(leftLeg.model, leftLeg.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    leftLeg.model = glm::scale(leftLeg.model, leftLeg.scale);

    rightLeg.position = body.position;
    rightLeg.scale = body.scale;
    rightLeg.rotation = body.rotation;
    rightLeg.object = new Object(objDir + "steve/right_legv2.obj");
    rightLeg.object->load_to_buffer();
    rightLeg.object->load_texture(textureDir + "steve.png");
    rightLeg.model = glm::mat4(1.0f);
    rightLeg.model = glm::translate(rightLeg.model, rightLeg.position);
    rightLeg.model = glm::rotate(rightLeg.model, rightLeg.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    rightLeg.model = glm::scale(rightLeg.model, rightLeg.scale);
}

void Steve::render(shader_program_t* shader, const glm::mat4& view, const glm::mat4& projection){
    shader->use();
    
    // Set matrix for view, projection, model transformation
    shader->set_uniform_value("view", view);
    shader->set_uniform_value("projection", projection);

    // Set material properties
    shader->set_uniform_value("isDead", isDead);

    // Render body
    shader->set_uniform_value("model", body.model);
    body.object->render();

    // Render head
    shader->set_uniform_value("model", head.model);
    head.object->render();

    // Render hands
    shader->set_uniform_value("model", leftHand.model);
    leftHand.object->render();
    shader->set_uniform_value("model", rightHand.model);
    rightHand.object->render();

    // Render legs
    shader->set_uniform_value("model", leftLeg.model);
    leftLeg.object->render();
    shader->set_uniform_value("model", rightLeg.model);
    rightLeg.object->render();
    
    shader->release();
}

void Steve::update() {
    if (isDead){
        // 如果死亡，整個身體要向後倒下
        glm::vec3 deathRotationVec = glm::vec3(0.0f, 0.0f, -1.0f);

        // 更新死亡動畫
        if (deathRotation < 90.0f) {
            deathRotation = std::min(90.0f, deathRotation + DEATH_ROTATION_SPEED);
        } 

        // Body transformation
        body.model = glm::mat4(1.0f);
        body.model = glm::translate(body.model, body.position);
        body.model = glm::rotate(body.model, glm::radians(deathRotation), deathRotationVec);  // 死亡旋轉
        body.model = glm::scale(body.model, body.scale);

        // Head transformation
        head.model = glm::mat4(1.0f);
        head.model = glm::translate(head.model, head.position);
        head.model = glm::rotate(head.model, glm::radians(deathRotation), deathRotationVec);  // 死亡旋轉
        head.model = glm::scale(head.model, head.scale);

        // Left hand transformation
        leftHand.model = glm::mat4(1.0f);
        leftHand.model = glm::translate(leftHand.model, leftHand.position);
        leftHand.model = glm::rotate(leftHand.model, glm::radians(deathRotation), deathRotationVec);  // 死亡旋轉
        leftHand.model = glm::scale(leftHand.model, leftHand.scale);

        // Right hand transformation
        rightHand.model = glm::mat4(1.0f);
        rightHand.model = glm::translate(rightHand.model, rightHand.position);
        rightHand.model = glm::rotate(rightHand.model, glm::radians(deathRotation), deathRotationVec);  // 死亡旋轉
        rightHand.model = glm::scale(rightHand.model, rightHand.scale);

        // Left leg transformation
        leftLeg.model = glm::mat4(1.0f);
        leftLeg.model = glm::translate(leftLeg.model, leftLeg.position);
        leftLeg.model = glm::rotate(leftLeg.model, glm::radians(deathRotation), deathRotationVec);  // 死亡旋轉
        leftLeg.model = glm::scale(leftLeg.model, leftLeg.scale);

        // Right leg transformation
        rightLeg.model = glm::mat4(1.0f);
        rightLeg.model = glm::translate(rightLeg.model, rightLeg.position);
        rightLeg.model = glm::rotate(rightLeg.model, glm::radians(deathRotation), deathRotationVec);  // 死亡旋轉
        rightLeg.model = glm::scale(rightLeg.model, rightLeg.scale);
    }
    // 更新揮手動畫
    else if (isSwinging) {
        const float SWING_SPEED = 3.0f;  // 基礎揮動速度

        if (swingForward) {
            // 計算動畫進度 (0.0 到 1.0)
            float progress = swingRotation / MAX_SWING_ANGLE;
            
            // 使用 smooth step function 來創造慢->快->慢的效果
            float easeSpeed = SWING_SPEED * (1.2f - (progress * progress));
            swingRotation = std::min(MAX_SWING_ANGLE, swingRotation + easeSpeed);
            
            if (swingRotation >= MAX_SWING_ANGLE) {
                swingForward = false;
            }
        } else {
            // 計算返回動畫進度 (1.0 到 0.0)
            float progress = 1.0f - (swingRotation / MAX_SWING_ANGLE);
            
            // 使用相同的 easing function 來返回
            float easeSpeed = SWING_SPEED * (1.2f - (progress * progress));
            swingRotation = std::max(0.0f, swingRotation - easeSpeed);
            
            if (swingRotation <= 0.0f) {
                isSwinging = false;
                swingForward = true;
            }
        }

        // 更新手臂模型
        leftHand.model = glm::mat4(1.0f);
        leftHand.model = glm::translate(leftHand.model, leftHand.position + lefthandOffset);
        leftHand.model = glm::rotate(leftHand.model, glm::radians(swingRotation * 0.7f), glm::vec3(0.0f, 1.0f, 0.0f));
        leftHand.model = glm::rotate(leftHand.model, glm::radians(swingRotation), glm::vec3(-1.0f, 0.0f, 0.0f));
        leftHand.model = glm::translate(leftHand.model, -lefthandOffset);
        leftHand.model = glm::scale(leftHand.model, leftHand.scale);
    }
    else {
        // 如果不在走路狀態，但手腳還沒回到原位，就執行回歸動畫
        if (!isWalking && !(
            leftHand.rotation.x == 0.0f && rightHand.rotation.x == 0.0f && 
            leftLeg.rotation.x == 0.0f && rightLeg.rotation.x == 0.0f)) {

            const float returnSpeed = swingSpeed * 45.0f;
            
            // 手臂回歸
            if (leftHand.rotation.x > 0.0f) {
                leftHand.rotation.x = std::max(0.0f, leftHand.rotation.x - returnSpeed);
            } else if (leftHand.rotation.x < 0.0f) {
                leftHand.rotation.x = std::min(0.0f, leftHand.rotation.x + returnSpeed);
            }
            
            if (rightHand.rotation.x > 0.0f) {
                rightHand.rotation.x = std::max(0.0f, rightHand.rotation.x - returnSpeed);
            } else if (rightHand.rotation.x < 0.0f) {
                rightHand.rotation.x = std::min(0.0f, rightHand.rotation.x + returnSpeed);
            }
            
            // 腿部回歸
            if (leftLeg.rotation.x > 0.0f) {
                leftLeg.rotation.x = std::max(0.0f, leftLeg.rotation.x - returnSpeed);
            } else if (leftLeg.rotation.x < 0.0f) {
                leftLeg.rotation.x = std::min(0.0f, leftLeg.rotation.x + returnSpeed);
            }
            
            if (rightLeg.rotation.x > 0.0f) {
                rightLeg.rotation.x = std::max(0.0f, rightLeg.rotation.x - returnSpeed);
            } else if (rightLeg.rotation.x < 0.0f) {
                rightLeg.rotation.x = std::min(0.0f, rightLeg.rotation.x + returnSpeed);
            }
        }
        animationTime += swingSpeed;

        // Update body model matrix
        body.model = glm::mat4(1.0f);
        body.model = glm::translate(body.model, body.position);
        body.model = glm::rotate(body.model, body.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        body.model = glm::scale(body.model, body.scale);
        
        // Update head model matrix with rotation
        head.model = glm::mat4(1.0f);
        head.model = glm::translate(head.model, head.position + headOffset);
        head.model = glm::rotate(head.model, glm::radians(headRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        head.model = glm::rotate(head.model, glm::radians(headRotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        head.model = glm::translate(head.model, -headOffset);
        head.model = glm::scale(head.model, head.scale);
        
        // Arm swing
        if (isWalking) {
            float armSwing = sin(animationTime) * 45.0f; // 45 degree swing angle
            leftHand.rotation.x = armSwing;
            rightHand.rotation.x = -armSwing;
        }
        
        // Update arm model matrix
        
        leftHand.model = glm::mat4(1.0f);
        leftHand.model = glm::translate(leftHand.model, leftHand.position + lefthandOffset);
        leftHand.model = glm::rotate(leftHand.model, glm::radians(leftHand.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        leftHand.model = glm::translate(leftHand.model, -lefthandOffset);
        leftHand.model = glm::scale(leftHand.model, leftHand.scale);
        
        rightHand.model = glm::mat4(1.0f);
        rightHand.model = glm::translate(rightHand.model, rightHand.position + righthandOffset);
        rightHand.model = glm::rotate(rightHand.model, glm::radians(rightHand.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        rightHand.model = glm::translate(rightHand.model, -righthandOffset);
        rightHand.model = glm::scale(rightHand.model, rightHand.scale);
        
        // Leg swing
        if (isWalking) {
            float legSwing = sin(animationTime) * 30.0f; // 30 degree swing angle
            leftLeg.rotation.x = -legSwing;
            rightLeg.rotation.x = legSwing;
        }
        
        // Update leg model matrix
        leftLeg.model = glm::mat4(1.0f);
        leftLeg.model = glm::translate(leftLeg.model, leftLeg.position);
        leftLeg.model = glm::rotate(leftLeg.model, glm::radians(leftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        leftLeg.model = glm::scale(leftLeg.model, leftLeg.scale);
        
        rightLeg.model = glm::mat4(1.0f);
        rightLeg.model = glm::translate(rightLeg.model, rightLeg.position);
        rightLeg.model = glm::rotate(rightLeg.model, glm::radians(rightLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        rightLeg.model = glm::scale(rightLeg.model, rightLeg.scale);
    }
    
    
}

void Steve::walk() {
    isWalking = !isWalking;
}

void Steve::moveForward() {
    // 更新所有部件的位置
    glm::vec3 movement = direction * moveSpeed;
    body.position += movement;
    head.position += movement;
    leftHand.position += movement;
    rightHand.position += movement;
    leftLeg.position += movement;
    rightLeg.position += movement;
    
    isWalking = true;  // 啟動走路動畫
}

void Steve::moveBackward() {
    glm::vec3 movement = -direction * moveSpeed;
    body.position += movement;
    head.position += movement;
    leftHand.position += movement;
    rightHand.position += movement;
    leftLeg.position += movement;
    rightLeg.position += movement;
    
    isWalking = true;
}

void Steve::moveLeft() {
    glm::vec3 movement = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction)) * moveSpeed;
    body.position += movement;
    head.position += movement;
    leftHand.position += movement;
    rightHand.position += movement;
    leftLeg.position += movement;
    rightLeg.position += movement;
    
    isWalking = true;
}

void Steve::moveRight() {
    glm::vec3 movement = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;
    body.position += movement;
    head.position += movement;
    leftHand.position += movement;
    rightHand.position += movement;
    leftLeg.position += movement;
    rightLeg.position += movement;
    
    isWalking = true;
}

void Steve::stopMoving() {
    isWalking = false;
}

void Steve::rotateHead(float xoffset, float yoffset) {
    headRotationY -= xoffset * HEAD_ROTATION_SENSITIVITY;
    headRotationX -= yoffset * HEAD_ROTATION_SENSITIVITY;
    
    // 限制頭部上下旋轉的角度範圍
    if(abs(headRotationX) > 45.0f)
        headRotationX = 45.0f * (headRotationX / abs(headRotationX));
    if(abs(headRotationY) > 45.0f)
        headRotationY = 45.0f * (headRotationY / abs(headRotationY));
}

glm::vec3 Steve::getEyePosition() const {
    // 眼睛位置在頭部上方略前方
    return head.position + glm::vec3(0.0f, 0.8f, 0.0f) + headOffset;
}

glm::vec3 Steve::getViewDirection() const {
    // 計算視線方向向量
    glm::vec3 direction;
    float adjustedRotationY = -headRotationY + 90.0f;  // 對-y軸旋轉90度
    direction.x = cos(glm::radians(adjustedRotationY)) * cos(glm::radians(-headRotationX));
    direction.y = sin(glm::radians(-headRotationX));
    direction.z = sin(glm::radians(adjustedRotationY)) * cos(glm::radians(-headRotationX));
    
    return glm::normalize(direction);
}
