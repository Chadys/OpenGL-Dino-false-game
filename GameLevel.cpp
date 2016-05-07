

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_level.h"

#include <fstream>
#include <sstream>


GameLevel::GameLevel(Texture3D bg) 
    : Bg(bg) { }

void GameLevel::Load(const GLchar *file)
{
    // Clear old data
    this->Obj.clear();
    // Load from file
    GLuint boxCode;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<GLint>> boxData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // Read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<GLint> row;
            while (sstream >> boxCode) // Read each word seperated by spaces
                row.push_back(boxCode);
            boxData.push_back(row);
        }
        if (boxData.size() > 0)
            this->init(boxData);
    }
}

void GameLevel::Draw(SpriteRenderer &renderer, SpriteRenderer &bgrenderer, glm::mat4 projection, glm::mat4 view)
{
    for (GameObject &box : this->Obj)
        box.Draw(renderer, projection, view);
    bgrenderer.DrawSprite(this->Bg, projection, view);
}

void GameLevel::init(std::vector<std::vector<GLint>> boxData)
{
    // Calculate dimensions
    GLuint height = boxData.size();
    GLuint width = boxData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
    // Initialize level boxs based on boxData		
    for (GLuint y = 0; y < height; ++y)
    {
        for (GLuint x = 0; x < width; ++x)
        {
            // Check block type from level data (2D level array)
            if (boxData[y][x] > 0 && boxData[y][x] < 10) // Block
            {
                GLfloat size(1);
                glm::vec3 pos(x*size,(boxData[y][x]-5)*(size/2),-(y*size));
                GameObject obj(pos, glm::vec3(size), ResourceManager::GetTexture("fence"), glm::vec3(1.0f, 0.0f, 0.0f));
                this->Obj.push_back(obj);
            }
        }
    }
}

