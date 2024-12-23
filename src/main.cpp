#include <bits/stdc++.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "header/cube.h"
#include "header/stb_image.h"
#include "header/shader.h"
#include "header/object.h"
#include "header/Steve.h"
#include "header/Chest.h"
#include "header/creeper.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int loadCubemap(std::vector<string> &mFileName);

struct material_t{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float gloss;
};

struct light_t{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct model_t{
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::mat4 model;
    Object* object;
};

struct camera_t{
    glm::vec3 position;
    glm::vec3 up;
    float rotationY;

    camera_t() :
        position(glm::vec3(0.0f, 0.0f, 5.0f)),  // 預設相機位置
        up(glm::vec3(0.0f, 1.0f, 0.0f)),        // 向上向量
        rotationY(0.0f)                          // 無旋轉
    {}
};

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// cube map 
unsigned int cubemapTexture;
unsigned int cubemapVAO, cubemapVBO;

// shader programs 
int shaderProgramIndex = 0;
std::vector<shader_program_t*> shaderPrograms;
shader_program_t* cubemapShader;

// additional dependencies
light_t light;
material_t material;
camera_t camera;
Steve* steve;
Chest* chest;
Creeper* creeper;

// model matrix
int moveDir = -1;
glm::mat4 cameraModel;



//////////////////////////////////////////////////////////////////////////
// Parameter setup, 
// You can change any of the settings if you want

void camera_setup(){
    camera.position = glm::vec3(0.0, 10.0, 100.0);
    camera.up = glm::vec3(0.0, 1.0, 0.0);
    camera.rotationY = 0;
}

void light_setup(){
    light.position = glm::vec3(0.0, 1000.0, 0.0);
    light.ambient = glm::vec3(0.6);
    light.diffuse = glm::vec3(1.0);
    light.specular = glm::vec3(1.5);
}

void material_setup(){
    material.ambient = glm::vec3(1.0);
    material.diffuse = glm::vec3(1.0);
    material.specular = glm::vec3(0.7);
    material.gloss = 10.5;
}
//////////////////////////////////////////////////////////////////////////

void model_setup(){

// Load the object and texture for each model here 

#if defined(__linux__) || defined(__APPLE__)
    std::string objDir = "../../src/asset/obj/";
    std::string textureDir = "../../src/asset/texture/";
#else
    std::string objDir = "..\\..\\src\\asset\\obj\\";
    std::string textureDir = "..\\..\\src\\asset\\texture\\";
#endif
    steve = new Steve;
    steve->setup(objDir, textureDir);

    chest = new Chest;
    chest->setup(objDir, textureDir);

    creeper = new Creeper();
    creeper->setup(objDir, textureDir);
}


void shader_setup(){

// Setup the shader program for each shading method

#if defined(__linux__) || defined(__APPLE__)
    std::string shaderDir = "../../src/shaders/";
#else
    std::string shaderDir = "..\\..\\src\\shaders\\";
#endif

    std::vector<std::string> shadingMethod = {
        "default",                              // default shading
        // "bling-phong", "gouraud", "metallic",   // addional shading effects (basic)
        // "glass_schlick", "glass_empricial",     // addional shading effects (advanced)
    };

    for(int i=0; i<shadingMethod.size(); i++){
        std::string vpath = shaderDir + shadingMethod[i] + ".vert";
        std::string gpath = shaderDir + shadingMethod[i] + ".geom";
        std::string fpath = shaderDir + shadingMethod[i] + ".frag";

        shader_program_t* shaderProgram = new shader_program_t();
        shaderProgram->create();
        shaderProgram->add_shader(vpath, GL_VERTEX_SHADER);
        shaderProgram->add_shader(gpath, GL_GEOMETRY_SHADER);
        shaderProgram->add_shader(fpath, GL_FRAGMENT_SHADER);
        shaderProgram->link_shader();
        shaderPrograms.push_back(shaderProgram);
    } 
}


void cubemap_setup(){

// Setup all the necessary things for cubemap rendering
// Including: cubemap texture, shader program, VAO, VBO

#if defined(__linux__) || defined(__APPLE__)
    std::string cubemapDir = "../../src/asset/texture/skybox/";
    std::string shaderDir = "../../src/shaders/";
#else
    std::string cubemapDir = "..\\..\\src\\asset\\texture\\skybox\\";
    std::string shaderDir = "..\\..\\src\\shaders\\";
#endif

    // setup texture for cubemap
    std::vector<std::string> faces
    {
        cubemapDir + "right.jpg",
        cubemapDir + "left.jpg",
        cubemapDir + "top.jpg",
        cubemapDir + "bottom.jpg",
        cubemapDir + "front.jpg",
        cubemapDir + "back.jpg"
    };
    cubemapTexture = loadCubemap(faces);   

    // setup shader for cubemap
    std::string vpath = shaderDir + "cubemap.vert";
    std::string fpath = shaderDir + "cubemap.frag";
    
    cubemapShader = new shader_program_t();
    cubemapShader->create();
    cubemapShader->add_shader(vpath, GL_VERTEX_SHADER);
    cubemapShader->add_shader(fpath, GL_FRAGMENT_SHADER);
    cubemapShader->link_shader();

    glGenVertexArrays(1, &cubemapVAO);
    glGenBuffers(1, &cubemapVBO);
    glBindVertexArray(cubemapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), &cubemapVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
}

void setup(){

    // Initialize shader model camera light material
    light_setup();
    model_setup();
    shader_setup();
    camera_setup();
    cubemap_setup();
    material_setup();

    // Enable depth test, face culling ...
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Debug: enable for debugging
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback([](  GLenum source, GLenum type, GLuint id, GLenum severity, 
    //                             GLsizei length, const GLchar* message, const void* userParam) {

    // std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") 
    //           << "type = " << type 
    //           << ", severity = " << severity 
    //           << ", message = " << message << std::endl;
    // }, nullptr);
}

void update(){
    camera.rotationY = (camera.rotationY > 360.0) ? 0.0 : camera.rotationY;
    cameraModel = glm::mat4(1.0f);
    cameraModel = glm::rotate(cameraModel, glm::radians(camera.rotationY), camera.up);
    cameraModel = glm::translate(cameraModel, camera.position);
}

void render(){

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Calculate view, projection matrix
    glm::mat4 view = glm::lookAt(glm::vec3(cameraModel[3]), glm::vec3(0.0), camera.up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    
    steve->update();
    steve->render(shaderPrograms[shaderProgramIndex], view, projection);

    chest->update();
    chest->render(shaderPrograms[shaderProgramIndex], view, projection);

    creeper->update();
    creeper->render(shaderPrograms[shaderProgramIndex], view, projection);

    glm::vec3 eye = glm::vec3(cameraModel[3]);
    // Set uniform value for Blin-Phong shader and Gouraud shader
    // if (shaderProgramIndex == 1 || shaderProgramIndex == 2) {
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("lightPos", light.position);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("ambientColor", material.ambient);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("ambientLight", light.ambient);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("diffuseColor", material.diffuse);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("diffuseLight", light.diffuse);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("specularColor", material.specular);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("specularLight", light.specular);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("gloss", material.gloss);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("cameraPos", eye);
    // }
    // else if (shaderProgramIndex == 3) {
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("lightPos", light.position);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("cameraPos", eye);

    //     // Bind skybox texture
    //     glActiveTexture(GL_TEXTURE1);
    //     glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("skybox", 1);
    // }
    // else if (shaderProgramIndex == 4 || shaderProgramIndex == 5) {
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("cameraPos", eye);
    //      // Bind skybox texture
    //     glActiveTexture(GL_TEXTURE1);
    //     glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    //     shaderPrograms[shaderProgramIndex]->set_uniform_value("skybox", 1);
    // }
    
    

    // TODO 4-2 
    // Rendering cubemap environment
    // Hint:
    // 1. All the needed things are already set up in cubemap_setup() function.
    // 2. You can use the vertices in cubemapVertices provided in the header/cube.h
    // 3. You need to set the view, projection matrix.
    // 4. Use the cubemapShader to render the cubemap 
    //    (refer to the above code to get an idea of how to use the shader program)
    
    // Render skybox last
    cubemapShader->use();
    
    // Remove translation from view matrix
    glm::mat4 viewWithoutTranslation = glm::mat4(glm::mat3(view));
    
    // Set uniforms for cubemap shader
    cubemapShader->set_uniform_value("view", viewWithoutTranslation);
    cubemapShader->set_uniform_value("projection", projection);
    
    // Draw skybox
    glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
    glBindVertexArray(cubemapVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // Set depth function back to default
    
    cubemapShader->release();
}


int main() {
    // 關閉 iostream 的同步，提高性能
    std::ios_base::sync_with_stdio(false);
    
    std::cout << "Starting program..." << std::endl;
    std::cout.flush();  // 強制輸出
    
    // glfw: initialize and configure
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::cerr.flush();  // 強制輸出錯誤訊息
        return -1;
    }
    std::cout << "GLFW initialized" << std::endl;
    std::cout.flush();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW4-111550093", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << "Window created" << std::endl;
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "GLAD initialized" << std::endl;

    try {
        std::cout << "Starting setup..." << std::endl;
        std::cout.flush();
        setup();
        std::cout << "Setup completed" << std::endl;
        std::cout.flush();
    } catch (const std::exception& e) {
        std::cerr << "Setup failed: " << e.what() << std::endl;
        std::cerr.flush();
        glfwTerminate();
        return -1;
    }

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        try {
            update();
            render();
        } catch (const std::exception& e) {
            std::cerr << "Render loop error: " << e.what() << std::endl;
            break;
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Add key callback
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    // The action is one of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE.
    // Events with GLFW_PRESS and GLFW_RELEASE actions are emitted for every key press.
    // Most keys will also emit events with GLFW_REPEAT actions while a key is held down.
    // https://www.glfw.org/docs/3.3/input_guide.html

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // shader program selection
    // if (key == GLFW_KEY_0 && (action == GLFW_REPEAT || action == GLFW_PRESS)) 
    //     shaderProgramIndex = 0;
    // if (key == GLFW_KEY_1 && (action == GLFW_REPEAT || action == GLFW_PRESS)) 
    //     shaderProgramIndex = 1;
    // if (key == GLFW_KEY_2 && (action == GLFW_REPEAT || action == GLFW_PRESS)) 
    //     shaderProgramIndex = 2;
    // if (key == GLFW_KEY_3 && (action == GLFW_REPEAT || action == GLFW_PRESS))
    //     shaderProgramIndex = 3;
    // if (key == GLFW_KEY_4 && (action == GLFW_REPEAT || action == GLFW_PRESS))
    //     shaderProgramIndex = 4;
    // if (key == GLFW_KEY_5 && (action == GLFW_REPEAT || action == GLFW_PRESS))
    //     shaderProgramIndex = 5;

    // camera movement
    float cameraSpeed = 0.5f;
    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.position.z -= 10.0;
    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.position.z += 10.0;
    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.rotationY -= 10.0;
    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.rotationY += 10.0;
    
    if (key == GLFW_KEY_SPACE && (action == GLFW_REPEAT || action == GLFW_PRESS))
        steve->walk();
    
    if (key == GLFW_KEY_O && (action == GLFW_REPEAT || action == GLFW_PRESS))
        chest->open();
    if (key == GLFW_KEY_C && (action == GLFW_REPEAT || action == GLFW_PRESS))
        chest->close();

    if (key == GLFW_KEY_L && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        creeper->rotateHead(-5.0f);  // Turn head left
    }
    if (key == GLFW_KEY_J && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        creeper->rotateHead(5.0f);   // Turn head right
    }
    
    if (key == GLFW_KEY_H && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        creeper->toggleScaleAndShimmer();
    }

    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        creeper->toggleWalking();
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

// Loading cubemap texture
unsigned int loadCubemap(vector<std::string>& faces){

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(false);
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}
