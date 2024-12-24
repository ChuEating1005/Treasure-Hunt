#include "header/Grass.h"
#include <unistd.h> 
#include <bits/stdc++.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Grass::Grass() {
    scaleRatio = 3.0f;
}

Grass::~Grass() {
    delete grass.object;
    
}

void Grass::setup(const string& objDir, const string& textureDir){
    
    grass.position = glm::vec3(0.0f, 0.0f, 0.0f);
    grass.scale = glm::vec3(scaleRatio * 1.0f, scaleRatio * 1.0f, scaleRatio * 1.0f);
    grass.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    
    grass.object = new Object(objDir + "grass/grass.obj");
    grass.object->load_to_buffer();
    grass.object->load_texture(textureDir + "grass.png"); 
    
    grass.model = glm::mat4(1.0f);
    grass.model = glm::translate(grass.model, grass.position);
    grass.model = glm::rotate(grass.model, grass.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    grass.model = glm::scale(grass.model, grass.scale);
}

void Grass::render(shader_program_t* shader, const glm::mat4& view, const glm::mat4& projection){
    shader->use();
    
    // Set matrix for view, projection, model transformation
    shader->set_uniform_value("view", view);
    shader->set_uniform_value("projection", projection);

    // Set material properties
    shader->set_uniform_value("isShimmering", false);
    shader->set_uniform_value("explodeFactor", 0.0f);
    shader->set_uniform_value("whiteFlash", false);

    shader->set_uniform_value("model", grass.model);
    grass.object->render();

    shader->release();
}

void Grass::update() {
    grass.model = glm::mat4(1.0f);
    grass.model = glm::translate(grass.model, grass.position);
    grass.model = glm::rotate(grass.model, glm::radians(grass.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));        
    grass.model = glm::scale(grass.model, grass.scale);
}

