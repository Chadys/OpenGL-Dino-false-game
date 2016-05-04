

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"
#include "texture.h"


/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
    // Level state
    std::vector<GameObject> Obj;
    Texture3D Skybox;
    // Constructor
    GameLevel(Texture3D skybox);
    // Loads level from file
    void      Load(const GLchar *file);
    // Render level
    void      Draw(SpriteRenderer &renderer, SpriteRenderer &skyrenderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
private:
    // Initialize level from tile data
    void      init(std::vector<std::vector<GLint>> tileData);
};

#endif

