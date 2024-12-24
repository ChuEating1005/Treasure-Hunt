#include "header/Ground.h"
#include "header/stb_image.h"
#include <iostream>

Ground::Ground() : VAO(0), VBO(0), texture(0), model(1.0f), alpha(0.5f) {} // Initialized alpha

Ground::~Ground() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
}

void Ground::setup(const std::string& textureDir) {
    // Define vertices with texture coordinates
    float groundVerticesWithTexture[] = {
        // Position           // Texture coords
        -50.0f, -1.0f,  50.0f,  0.0f,   0.0f,
        50.0f, -1.0f,  50.0f,   50.0f,  0.0f,
        50.0f, -1.0f, -50.0f,   50.0f,  50.0f,
        50.0f, -1.0f, -50.0f,   50.0f,  50.0f,
        -50.0f, -1.0f, -50.0f,  0.0f,   50.0f,
        -50.0f, -1.0f,  50.0f,  0.0f,   0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVerticesWithTexture), groundVerticesWithTexture, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glBindVertexArray(0);

    // Texture setup
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::string texturePath = textureDir + "ground.png";
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load ground texture at path: " << texturePath << std::endl;
    }
    stbi_image_free(data);
}

void Ground::update() {
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
}

void Ground::render(shader_program_t* shader, glm::mat4& view, glm::mat4& projection) {
    shader->use();
    shader->set_uniform_value("view", view);
    shader->set_uniform_value("projection", projection);
    shader->set_uniform_value("model", model);
    

    // Set material properties
    shader->set_uniform_value("isShimmering", false);
    shader->set_uniform_value("explodeFactor", 0.0f);
    shader->set_uniform_value("whiteFlash", false);
    shader->set_uniform_value("alpha", alpha); // Set alpha uniform
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader->set_uniform_value("ourTexture", 0);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
