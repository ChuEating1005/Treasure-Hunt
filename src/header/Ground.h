#ifndef GROUND_H
#define GROUND_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include <string>

class Ground {
public:
    Ground();
    ~Ground();
    void setup(const std::string& textureDir);
    void update();
    void render(shader_program_t* shaderProgram, glm::mat4& view, glm::mat4& projection);

private:
    unsigned int VAO, VBO;
    unsigned int texture;
    glm::mat4 model;
    float alpha; // Added alpha transparency
};

#endif
