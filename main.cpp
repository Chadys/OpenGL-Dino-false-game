// C Library Includes
#include <signal.h>

// Std. Includes
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// SDL_Mixer
#include <SDL/SDL_mixer.h>

// My includes
#include "game.h"
#include "resource_manager.h"
#include "Camera.h"

// Function prototypes
void sigint_handler(int sig);
void printFps();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void simple_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void endmusic_callback();

// Properties
Game game;
bool record = false;
std::fstream prog;

// The MAIN function, from here we start our application and run our Game loop
int main (int argc, char *argv[])
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

    // Init SDL_Mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{
		printf("%s", Mix_GetError());
	}

    // Window
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "AWESOME RAPTOR GAME", glfwGetPrimaryMonitor(), nullptr); // Windowed
    glViewport(0, 0, screenWidth, screenHeight);
    glfwMakeContextCurrent(window);

    // Recording 
    GLfloat prog_time = 0;
    std::istringstream prog_reader;

    if (argc > 1 && !std::string("record").compare(argv[1]))
    	record = true;
    if(record)
    	prog.open("levels/schedule.prg", ios::out | ios::trunc);
    else
    	prog.open("levels/schedule.prg", ios::in);
    if (!prog.is_open())
    	std::cout << "ERROR::STREAM: Could not open file." << std::endl;

    else if (!record){
    	std::string line;
        if (std::getline(prog, line)){ // Read first line of prog
        	prog_reader.str(line);
        	prog_reader >> prog_time; // Get the first word in line (the time)
        }
    }

    // Set the required callback functions
    signal(SIGINT, sigint_handler);

    if(record){
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
    }
    else
        glfwSetKeyCallback(window, simple_key_callback);
    Mix_HookMusicFinished(endmusic_callback);

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

    // Time variables
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

        // Read record
    	if (!record){
    		while (prog_time && prog_time <= currentFrame){
    			char type;
    			if (prog_reader >> type){
    				switch(type){
    					case 'k': //Keyboard event
    						int key, action;
    						if (prog_reader >> key && prog_reader >> action){
    							action = action ? GLFW_PRESS : GLFW_RELEASE;
    							key_callback(window, key, 0, action, 0);
    						}
    						break;
    					case 's': //scroll event
    						double yoffset;
    						if (prog_reader >> yoffset)
    							scroll_callback(window, 0, yoffset);
    						break;
    					case 'm': //mouse event
    						double xpos, ypos;
    						if (prog_reader >> xpos && prog_reader >> ypos)
    							mouse_callback(window, xpos, ypos);
    						break;
    					default:
    						break;
    				}
    			}
    			prog_time = 0;
    			std::string line;
        		if (std::getline(prog, line)){
        			prog_reader.str(line);
        			prog_reader.clear();
        			prog_reader >> prog_time;
        		}
    		}
		}


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
    if (prog.is_open())
    	prog.close();
    ResourceManager::Clear();
	Mix_CloseAudio();
    glfwTerminate();
    return 0;
}


// Is called whenever a key is pressed/released via GLFW
void simple_key_callback(GLFWwindow* window, int key, __attribute__((unused)) int scancode, int action, __attribute__((unused)) int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
void key_callback(GLFWwindow* window, int key, __attribute__((unused)) int scancode, int action, __attribute__((unused)) int mods)
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
    //Write record
    if (record && prog.is_open()){
    	if (action == GLFW_PRESS)
    		prog << glfwGetTime() << " k " << (int)key << " " << 1 << std::endl;
    	else if(action == GLFW_RELEASE)
    		prog << glfwGetTime() << " k " << (int)key << " " << 0 << std::endl;
    }
}

void mouse_callback(__attribute__((unused)) GLFWwindow* window, double xpos, double ypos)
{
    game.ProcessMouseMovement(xpos, ypos);

    //Write record
    if (record && prog.is_open() && game.State == GAME_3D)
    	prog << glfwGetTime() << " m " << xpos << " " << ypos << std::endl;
}   


void scroll_callback(__attribute__((unused)) GLFWwindow* window, __attribute__((unused)) double xoffset, double yoffset)
{
    game.ProcessMouseScroll(yoffset);

    //Write record
    if (record && prog.is_open() && game.State == GAME_3D)
    	prog << glfwGetTime() << " s " << yoffset << std::endl;
}

void endmusic_callback(){
	game.ProcessEndingMusic();
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


void sigint_handler(__attribute__((unused)) int sig) {
    if (prog.is_open())
    	prog.close();
    ResourceManager::Clear();
	Mix_CloseAudio();
    glfwTerminate();
    exit(1);
}