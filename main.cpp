

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
const GLuint screenWidth = 800, screenHeight = 600;

Game game(screenWidth,screenHeight);

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

    // Window
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "AWESOME CAT GAME", nullptr, nullptr); // Windowed
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
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE); 
    glLineWidth(5.0f); 

    // DeltaTime variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Initialize game
    game.Init();
    // Start Game within Menu State
    game.State = GAME_ACTIVE;


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
        glClearColor(0.4f, 0.0f, 0.8f, 1.0f);
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
            game.Keys[key] = true;
        else if(action == GLFW_RELEASE)
            game.Keys[key] = false;  
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