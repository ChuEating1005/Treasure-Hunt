#include "header/Chest.h"
#include <unistd.h> 
#include <bits/stdc++.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Chest::Chest() {
    // Initialize animation variables
    animationTime = 0.0f;
    openSpeed = 4.0f;
    isOpen = false;
    scaleRatio = 5.0f;
}

Chest::~Chest() {
    delete lid.object;
    delete container.object;
}

void Chest::setup(const string& objDir, const string& textureDir){
    
    container.position = glm::vec3(0.0f, 7.0f, 0.0f);
    container.scale = glm::vec3(scaleRatio * 1.0f, scaleRatio * 1.0f, scaleRatio * 1.0f);
    container.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    container.object = new Object(objDir + "chest/container.obj");
    container.object->load_to_buffer();
    container.object->load_texture(textureDir + "chest.png");
    container.model = glm::mat4(1.0f);
    container.model = glm::translate(container.model, container.position);
    container.model = glm::scale(container.model, container.scale);

    lid.position = container.position;
    lid.scale = container.scale;
    lid.rotation = container.rotation;
    lid.object = new Object(objDir + "chest/lid.obj");
    lid.object->load_to_buffer();
    lid.object->load_texture(textureDir + "chest.png");
    lid.model = glm::mat4(1.0f);
    lid.model = glm::translate(lid.model, lid.position);
    lid.model = glm::scale(lid.model, lid.scale);
}

void Chest::render(shader_program_t* shader, const glm::mat4& view, const glm::mat4& projection){
    shader->use();
    
    // Set matrix for view, projection, model transformation
    shader->set_uniform_value("view", view);
    shader->set_uniform_value("projection", projection);

    shader->set_uniform_value("model", container.model);
    container.object->render();
    shader->set_uniform_value("model", lid.model);
    lid.object->render();

    shader->release();
}

void Chest::update() {
    // 箱子蓋的最大開啟角度
    const float MAX_LID_ANGLE = 85.0f;
    if (isOpen)    
        // 更新箱子蓋的旋轉
        if (lid.rotation.x < MAX_LID_ANGLE) {
            // 計算動畫進度 (0.0 到 1.0)
            float progress = lid.rotation.x / MAX_LID_ANGLE;
            
            // 使用 smooth step function 來創造慢->快->慢的效果
            float easeSpeed = openSpeed * (1.1f - (progress * progress)) * (1.1f - (progress * progress));
            lid.rotation.x += easeSpeed;  // 使用 eased speed
            
            // 更新箱子蓋的模型矩陣
            glm::vec3 lidHingeOffset = scaleRatio * glm::vec3(-0.9f, 1.1f, 0.0f);
            lid.model = glm::mat4(1.0f);
            lid.model = glm::translate(lid.model, lid.position + lidHingeOffset);
            lid.model = glm::rotate(lid.model, glm::radians(lid.rotation.x), glm::vec3(0.0f, 0.0f, 1.0f));
            lid.model = glm::translate(lid.model, -lidHingeOffset);
            lid.model = glm::scale(lid.model, lid.scale);
        }
        else {
            isOpen = false;
        }
    else if (isClose){
        if (lid.rotation.x > 0.0f) {
            // 計算動畫進度 (0.0 到 1.0)
            float progress = (MAX_LID_ANGLE - lid.rotation.x) / MAX_LID_ANGLE;
            
            // 使用 smooth step function 來創造慢->快->慢的效果
            float easeSpeed = openSpeed * (1.2f - (progress * progress)) * (1.2f - (progress * progress));
            lid.rotation.x -= easeSpeed;  // 使用 eased speed
            
            // 更新箱子蓋的模型矩陣
            glm::vec3 lidHingeOffset = scaleRatio * glm::vec3(-0.9f, 1.1f, 0.0f);
            lid.model = glm::mat4(1.0f);
            lid.model = glm::translate(lid.model, lid.position + lidHingeOffset);
            lid.model = glm::rotate(lid.model, glm::radians(lid.rotation.x), glm::vec3(0.0f, 0.0f, 1.0f));
            lid.model = glm::translate(lid.model, -lidHingeOffset);
            lid.model = glm::scale(lid.model, lid.scale);
        }
        else {
            isClose = false;
        }
    }
}

void Chest::open() {
    isOpen = true;
}

void Chest::close() {
    isClose = true;
}