// Std. Includes
#include <string>
#include<Windows.h>
// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model_MultiTex.h"

// Other Libs
#include <SOIL2/SOIL2.h>

// Properties
const GLuint WIDTH = 1366, HEIGHT = 710;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void MovimientoVirus( );
void MovimientoLinfo1( );
void MovimientoLinfo2();
void MatarVirus();
void MovimientoGlobulo();
void Reiniciar();


// Camera
Camera camera( glm::vec3( -17.0f, 0.0f, 0.0f ),glm::vec3(0.0f,1.0f,0.0f),0.0f);
bool keys[1024];
GLfloat lastX = WIDTH/2, lastY = HEIGHT/2;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//Mueven al virus
float Xvirus1 = 40.0;
float Xvirus2 = 40.0;
float Xvirus3 = 40.0;
//Mueven al linfo
float Ylinfo1 = 20;
float Zlinfo1 = -30;
float Zlinfo2 = 20;
//MuereLinfo
float escala = 1; 

//Mueven Globulo
float Xglo = 30;
float Yglo;

bool es2 = false;
bool ActivaVirus = false;
bool ActivalinfoP1 = false; 
bool ActivalinfoP2 = false;
bool ActivalinfoP3 = false;
bool ActivaGlobulo = false;
bool DesVirus = false; 


int main( )
{
    FreeConsole();
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Proyecto: vacuna - Escena 2 ", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    // Setup and compile our shaders
    Shader shader( "Shaders/modelLoading.vs", "Shaders/modelLoading.frag" );
    Shader phong_texture("Shaders/dirlight.vs", "Shaders/dirlight.frag");
    Shader fresnel("Shaders/fresnel.vs", "Shaders/fresnel.frag");
    Shader bump("Shaders/bump.vs", "Shaders/bump.frag");
    // Load models
    Model organismo((char *)"Models/Escena2/vena.obj");
    Model linfocito((char*)"Models/Escena2/linfocito.obj");
    Model globulo((char*)"Models/Escena2/globulo.obj");
    Model globuloRo((char*)"Models/Escena2/globuloRo.obj");
    ModelMultiTex virus((char*)"Models/Escena2/virus.obj");
    Shader s_virus;
    Shader s_organismo;
    Shader s_linfocito;
    Shader s_globulo;
   
    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        s_globulo = phong_texture;

        glm::mat4 model = glm::mat4(1);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        if (ActivaVirus)
        {
            MovimientoVirus();
        }
        if (ActivalinfoP1)
        {
            MovimientoLinfo1();
        }
        if (ActivalinfoP2)
        {
            MovimientoLinfo2();
        }
        if (ActivaGlobulo)
        {
            MovimientoGlobulo();
        }

        // Draw the loaded model

        //*********ESCENA 2*********
        phong_texture.Use();
        glUniformMatrix4fv(glGetUniformLocation(phong_texture.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(phong_texture.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(phong_texture.Program, "dirLight.direction"), 1.0f, -1.0f, 0.0f);
        glUniform3f(glGetUniformLocation(phong_texture.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(phong_texture.Program, "dirLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(phong_texture.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(phong_texture.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform1f(glGetUniformLocation(phong_texture.Program, "opacity"), 1.0f);

          
        //Organismos
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));	// It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(phong_texture.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        //organismo.Draw(shader);
        organismo.Draw(phong_texture);
        bump.Use();
        //model = glm::mat4(1);
        glm::vec3 light(1.0f, 1.0f, 1.0f);
        glm::vec3 light2(1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(glGetUniformLocation(bump.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(bump.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(bump.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(bump.Program, "dirLight.direction"),
        1.0f, -1.0f, 0.0f);
        //    camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(bump.Program, "dirLight.ambient"), light2.x, light2.y, light2.z);
        glUniform3f(glGetUniformLocation(bump.Program, "dirLight.diffuse"), light.x, light.y, light.z);
        glUniform3f(glGetUniformLocation(bump.Program, "dirLight.specular"), light.x, light.y, light.z);
        glUniform3f(glGetUniformLocation(bump.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        //organismo.Draw(bump);

        s_virus = bump;
        s_virus.Use();
        //Virus
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xvirus1, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(escala));
        glUniformMatrix4fv(glGetUniformLocation(s_virus.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        if (DesVirus == false)
        {
            virus.Draw(s_virus);
        }
        

        //Virus2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xvirus2, 2.0f, 3.5f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(escala));
        glUniformMatrix4fv(glGetUniformLocation(s_virus.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        if (DesVirus == false)
        {
            virus.Draw(s_virus);
        }

        //Virus3
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xvirus3, -2.0, -3.5f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(escala));
        glUniformMatrix4fv(glGetUniformLocation(s_virus.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        if (DesVirus == false)
        {
            virus.Draw(s_virus);
        }
        

        //LINFOCITOS
        s_linfocito = phong_texture;
        s_linfocito.Use();
        glUniform3f(glGetUniformLocation(phong_texture.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(s_linfocito.Program, "opacity"), 0.6f);
        glUniformMatrix4fv(glGetUniformLocation(s_linfocito.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(s_linfocito.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(s_linfocito.Program, "lightDir"), 0.0f, -1.0f, 0.0f);

        ////Linfocito
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, Zlinfo1, 0.0)); 
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_linfocito.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //linfocito.Draw(shader);

        linfocito.Draw(s_linfocito);

        ////Linfocito 2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.5f, 2.0f, Ylinfo1)); 
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_linfocito.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //linfocito.Draw(shader);
        linfocito.Draw(s_linfocito);


        //Linfocito3
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-6.5f, Zlinfo2, -3.5)); 
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_linfocito.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        linfocito.Draw(s_linfocito);
		
        s_globulo.Use();
        glUniform1f(glGetUniformLocation(s_globulo.Program, "opacity"), 1.0f);
        //Globulo
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo, 2, Yglo)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globulo.Draw(s_globulo);

        //Globulo 2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo * 0.5 - 1.0, 2, Yglo * 0.5 + 1.5)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globuloRo.Draw(s_globulo);

        //Globulo 3
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo * 0.25, -2, Yglo * 0.25)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globulo.Draw(s_globulo);

        //Globulo 4
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo * 0.25, 5, Yglo * 0.25)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globuloRo.Draw(s_globulo);

        //Globulo 5
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo, 2.5, Yglo - 1)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globulo.Draw(s_globulo);

        //Globulo 6
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo + 10, 2.5, Yglo - 1)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globuloRo.Draw(s_globulo);

        //Globulo 7
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo + 10, 5.5, Yglo - 0.1)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globulo.Draw(s_globulo);

        //Globulo 8 
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo + 1, 4.5, Yglo + 3)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globuloRo.Draw(s_globulo);

        //Globulo 9
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo + 20, 4.5, Yglo + 3)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globulo.Draw(s_globulo);

        //Globulo 10
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo - 15, 5.5, Yglo + 3)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globuloRo.Draw(s_globulo);

        //*************

         //Globulo 11
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo - 2, -2, Yglo + 1)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globulo.Draw(s_globulo);

        //Globulo 12
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo * 0.5 - 1.0, 3, Yglo * 0.5 + 1.5)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globuloRo.Draw(s_globulo);

        //Globulo 13
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo * 0.25, -3, Yglo * 0.25 - 1)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globulo.Draw(s_globulo);

        //Globulo 14
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo * 0.5, -5, Yglo + 2)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globuloRo.Draw(s_globulo);

        //Globulo 15
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo - 4, -1.5, Yglo - 1)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globulo.Draw(s_globulo);

        //Globulo 16
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo + 5, -2.5, Yglo + 2)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globuloRo.Draw(s_globulo);

        //Globulo 17
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo - 7, 5, Yglo - 2)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globulo.Draw(s_globulo);

        //Globulo 18 
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo - 2, -4.5, Yglo + 2)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globuloRo.Draw(s_globulo);

        //Globulo 19
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo - 5, 4.5, Yglo - 2)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globulo.Draw(s_globulo);

        //Globulo 20
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xglo - 5, 5.5, Yglo - 3)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(s_globulo.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globuloRo.Draw(s_globulo);
     
        
        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
    //Precionar tecla hacia adelante 
   
    if (keys[GLFW_KEY_1])
    {
        ActivaVirus = !ActivaVirus;
        ActivaGlobulo = !ActivaGlobulo;
        
    }
    if (keys[GLFW_KEY_2])
    {
        ActivalinfoP2 = !ActivalinfoP2;
    }
    if (keys[GLFW_KEY_3])
    {
        Reiniciar();
    }
 
}

void Reiniciar()
{
    Xvirus1 = 40;
    Xvirus2 = 40;
    Xvirus3 = 40;
    escala = 1;
    Ylinfo1 = 20;
    Zlinfo1 = -30;
    Zlinfo2 = 20;
    Xglo = 30;
    Yglo = 0; 
    ActivaVirus = false;
    ActivalinfoP1 = false;
    ActivalinfoP2 = false;
    ActivalinfoP3 = false;
    ActivaGlobulo = false;
    DesVirus = false;

}

void MovimientoVirus()
{
    if (ActivaVirus == true)
    {
        if (Xvirus1 > 0)
        {
            Xvirus1 -= 0.06;
        }
        if (Xvirus2 > -4.5)
        {
            Xvirus2 -= 0.035;
        }
        if (Xvirus3 > -6.5)
        {
            Xvirus3 -= 0.035;
        }
        if (Xvirus2 < -4.5)
        {
            ActivalinfoP1 = !ActivalinfoP1;
        }
    }
}

void MovimientoLinfo1()
{
    if (ActivalinfoP1 == true)
    {
        if (Zlinfo1 < - 3.0)
        {
            Zlinfo1 += 0.06;
        }
        if (Ylinfo1 > 6.5)
        {
            Ylinfo1 -= 0.035;
        }
        if (Zlinfo2 > 2.0)
        {
            Zlinfo2 -= 0.035;
        }
    }
}

void MovimientoLinfo2()
{
    if (ActivalinfoP2 == true)
    {
        if (Zlinfo1 < 0.0)
        {
            Zlinfo1 += 0.06;
        }
        if (Ylinfo1 > 3.5)
        {
            Ylinfo1 -= 0.035;
        }
        if (Zlinfo2 > -2.0)
        {
            Zlinfo2 -= 0.035;
        }
        MatarVirus();
        
        
    }
}

void MovimientoLinfo3()
{
    
    if (Zlinfo1 > -40)
    {
        Zlinfo1 -= 0.2;
    }
    if (Ylinfo1 < 30)
    {
        Ylinfo1 += 0.2;
    }
    if (Zlinfo2 < 40)
    {
        Zlinfo2 += 0.2;
    }
    
}

void MatarVirus()
{
    if (escala > -0.05)
    {
        escala -= 0.0005;
    }
    if (escala <= -0.050017)
    {
        DesVirus = true;
        MovimientoLinfo3();
    }
}

void MovimientoGlobulo()
{
    if (ActivaGlobulo == true)
    {
        if (Xglo >= -40)
        {
            Xglo -= 0.05;
        }


        if (Xglo <= -40)
        {
            Yglo = -25;
            Xglo = 40;
            Yglo = 0.0;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

