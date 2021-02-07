#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "modelAnim.h"

#include "MiAnimacion.h"

using namespace std;
int WIDTH = 1366, HEIGHT=780, SCREEN_WIDTH, SCREEN_HEIGHT;
Camera  camera(glm::vec3(-100.0f, 2.0f, -45.0f));
int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto final", nullptr, nullptr);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
    //CARGANDO MODELOS
    Animacion zombi(string("Animaciones/Personaje2/MacarenaDance.dae"), "Shaders/anim.vs", "Shaders/anim.frag");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        puts("Tiempo en main");
        printf("%f\n",glfwGetTime());
        puts("Tiempo en main termino");
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model(1);
        GLfloat currentFrame = glfwGetTime();
        puts("Hora de hacer el draw");
        zombi.draw(glm::value_ptr(view), glm::value_ptr(view), glm::value_ptr(projection));
        
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}