// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "game.h"
#include "resource_manager.h"
#include "Camera.h"

// Function prototypes
void printFps();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Properties
Game game;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWmonitor* primary;
    const GLFWvidmode* primary_mode;
    primary = glfwGetPrimaryMonitor();
    primary_mode = glfwGetVideoMode(primary);
    game.Width=primary_mode->width;
    game.Height=primary_mode->height;

    const GLuint screenWidth = primary_mode->width, screenHeight = primary_mode->height;


    // Window
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "AWESOME RAPTOR GAME", glfwGetPrimaryMonitor(), nullptr); // Windowed
    glViewport(0, 0, screenWidth, screenHeight);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();

    // OpenGL options
    //glEnable(GL_DEPTH_TEST); // only used for the 3D part
	//glEnable(GL_CULL_FACE); 
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(screenHeight/160); 

    // DeltaTime variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Initialize game
    game.Init();

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        printFps();
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //printf("%f\n", deltaTime);
        glfwPollEvents();

        // Manage user input
        game.ProcessInput(deltaTime);
    
        // Update Game state
        game.Update(deltaTime);
    
        // Render
        //glClearColor(0.4f, 0.0f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game.Render();
        	
	    // Swap the buffers
        glfwSwapBuffers(window);
    }
    // Delete all resources as loaded using the resource manager
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, __attribute__((unused)) int scancode, int action, __attribute__((unused)) int mode)
{
    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            game.Keys[key] = GL_TRUE;
        else if(action == GLFW_RELEASE){
            game.Keys[key] = GL_FALSE; 
            game.ProcessedKeys[key] = GL_FALSE; 
        }
    }
}

void mouse_callback(__attribute__((unused)) GLFWwindow* window, double xpos, double ypos)
{
    game.ProcessMouseMovement(xpos, ypos);
}   


void scroll_callback(__attribute__((unused)) GLFWwindow* window, __attribute__((unused)) double xoffset, double yoffset)
{
    game.ProcessMouseScroll(yoffset);
}

void printFps(void){
    float t;
    static float t0=0,f= 0;
    f++;
    t = glfwGetTime();
    if(t - t0 > 2.000) { 
        printf("%f\n", (f / (t - t0)));
        t0 = t;
        f  = 0; 
    }
}