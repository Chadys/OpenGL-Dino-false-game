

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "game_level.h"
#include "game_model.h"
#include "game_object.h"
#include <glm/glm.hpp>


// Represents the current state of the game
enum GameState {
    GAME_MENU,
    GAME_2D,
    GAME_3D
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
    GLuint                 Width, Height;

    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void ProcessMouseMovement(GLdouble xpos, GLdouble ypos);
    void ProcessMouseScroll(GLdouble yoffset);
    void Update(GLfloat dt);
    void Render();
    void Go2D();
    void Go3D();
private:
    Camera                 Cam;
    GLfloat                lastX, lastY;
    GLboolean              firstMouse;
    std::vector<GameLevel> Levels;
    GLuint                 Level;
    std::vector<GameModel> Models;
    std::vector<Object2D>  Sprites;
};

#endif

