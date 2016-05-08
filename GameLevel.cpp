

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
#include <cstdlib>     /* srand, rand */
#include <string>


GameLevel::GameLevel(std::unique_ptr<Tex> &bg) 
    : Bg(std::move(bg)) { }

void GameLevel::Load(const GLchar *file)
{
    std::vector<std::vector<GLint>> boxData = this->load(file);
    if (boxData.size() > 0)
        this->init(boxData);
}

void GameLevel::Load(const GLchar *file, GLuint width, GLuint height)
{
    std::vector<std::vector<GLint>> boxData = this->load(file);
    if (boxData.size() > 0)
        this->init(boxData, width, height);
}


void GameLevel::Draw(SpriteRenderer &renderer, SpriteRenderer &bgrenderer, glm::mat4 projection, glm::mat4 view)
{
    for (std::unique_ptr<GameObject> &box : this->Obj)
        box->Draw(renderer, projection, view);
    bgrenderer.DrawSprite(*this->Bg, projection, view);
}

void GameLevel::Draw(SpriteRenderer &renderer, GLuint width, GLuint height, glm::mat4 projection, glm::mat4 view)
{
    renderer.DrawSprite(*this->Bg, glm::vec2(0), glm::vec2(width, height), projection);
    for (std::unique_ptr<GameObject> &box : this->Obj)
       box->Draw(renderer, projection, view);
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
            if (boxData[y][x] > 0 && boxData[y][x] < 10) // Red Block
            {
                GLfloat size(1);
                glm::vec3 pos(x*size,(boxData[y][x]-5)*(size/2),-(y*size));
                this->Obj.push_back(std::unique_ptr<GameObject>(new Object3D(pos, glm::vec3(size), ResourceManager::GetTexture("fence"), glm::vec3(1.0f, 0.0f, 0.0f))));
            }
        }
    }
}

void GameLevel::init(std::vector<std::vector<GLint>> boxData, GLuint width, GLuint height)
{
    // Calculate dimensions
    GLuint n_block = boxData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
    // Initialize level boxs based on boxData
     for (GLuint i = 0 ; i < n_block ; i++){
         if (boxData[0][i] > 0) // Block
         {
             GLuint type_block = rand() % 6;
             Texture2D tex = ResourceManager::GetTexture("grass"+to_string(type_block));
             GLfloat x = (width/2-tex.Width/2) + i*tex.Width;
             GLfloat y = (height/2-tex.Height/2)-(((boxData[0][i]%10)-5)*tex.Height);
             this->Obj.push_back(std::unique_ptr<GameObject>(new Object2D(tex, glm::vec2(x,y))));

             if ((boxData[0][i] > 10 && boxData[0][i] < 20) || boxData[0][i] > 30){   // Fern 
                 tex = ResourceManager::GetTexture("fern");
                 this->Obj.push_back(std::unique_ptr<GameObject>(new Object2D(tex, glm::vec2(x,y-tex.Height))));
             }
             if (boxData[0][i] > 20){   // Vine 
                 tex = ResourceManager::GetTexture("vine");
                 this->Obj.push_back(std::unique_ptr<GameObject>(new Object2D(tex, glm::vec2(x,y))));
             }
         }
     }
}


std::vector<std::vector<GLint>> GameLevel::load(const GLchar *file)
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
    }
    return boxData;
}