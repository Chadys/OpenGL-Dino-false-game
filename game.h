/*******************************************************************
** Taken and adapted from learnopengl.com (part of a Breakout game) 
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Camera.h"
#include "game_level.h"
#include "game_object.h"
#include "state_manager.h"


// Represents the current state of the game
enum GameState {
    GAME_MENU,
    GAME_2D,
    GAME_3D
};

enum Target {
	NOP,
	CAM_JUMP,
	CAM_SLIDE,
	MODEL_JUMP
};

// Structure used for move using a Bezier's curve
struct Bezier {
    glm::vec3 depart;
    glm::vec3 middle;
    glm::vec3 arrivee;
    GLfloat	  time_elapsed;
    Target	  target;
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // Game state
    GameState              State;	
    GLboolean              Keys[1024];
    GLboolean              ProcessedKeys[1024];
    GLuint                 Width, Height;
    StateManager		   State_manager;

    // Constructor/Destructor
    Game();
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void ProcessMouseMovement(GLdouble xpos, GLdouble ypos);
    void ProcessMouseScroll(GLdouble yoffset);
    void ProcessEndingMusic();
    void Update(GLfloat dt);
    void Render();
    static glm::vec3 Update_Bezier(glm::vec3 depart,glm::vec3 middle,glm::vec3 arrivee,float t);
private:
    Camera                 				Cam;
    GLfloat                				lastX, lastY;
    GLboolean              				firstMouse;
    std::vector<GameLevel> 				Levels;
    GLuint                 				Level;
    std::vector<GameModel> 				Models;
    std::vector<Object2D>  				Sprites;
    Bezier				   				bezier;
    GLboolean			   				Selected_sprite;
    std::string							Music;			
    GLfloat								DinoSpin;		

    void GoMENU();
    void Go2D();
    void Go3D();
    void SetBezier(GLint x, GLint y);
    void SetBezier(glm::vec3 new_cam_pos);
    void ChangeVolume(GLfloat prev, GLfloat next);
};

#endif

