#include "Texture.h"
#include "modelAnim.h"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

class Animacion{
public:
    ModelAnim modelo=ModelAnim("Animaciones/Personaje2/MacarenaDance.dae");
    Shader shader;
    GLint modelLoc, viewLoc, projLoc;
    Animacion(string modelPath,string vertexPath,string fragmentPath) {
        shader = Shader("Shaders/anim.vs", "Shaders/anim.frag");
        modelo.initShaders(shader.Program);
        modelLoc = glGetUniformLocation(shader.Program, "model");
        viewLoc = glGetUniformLocation(shader.Program, "view");
        projLoc = glGetUniformLocation(shader.Program, "projection");
    }

    void draw(const GLfloat *model, const GLfloat *view, const GLfloat *projection) {
        shader.Use();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);

        glUniform3f(glGetUniformLocation(shader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "light.ambient"), 0.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "light.diffuse"), 0.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "light.specular"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(shader.Program, "light.direction"), 0.0f, -1.0f, -1.0f);
        puts("Tiempo en anim");
        printf("%f\n", glfwGetTime());
        puts("Tiempo en anim termino");
        modelo.Draw(shader);
        glBindVertexArray(0);
    }
};