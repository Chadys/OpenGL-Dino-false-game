

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Game-related State data
SpriteRenderer  *RendererBG;
SpriteRenderer  *RendererSprite;
SpriteRenderer  *Renderer3d;
SpriteRenderer  *RendererSkybox;


Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height), Cam(glm::vec3(0.0f, 0.0f, 3.0f)), lastX(400), lastY(300), firstMouse(true)
{ 
}

Game::~Game()
{
    delete RendererBG;
    delete RendererSprite;
    delete Renderer3d;
    delete RendererSkybox;
}

void Game::Init()
{
    // Load shaders
    Shader shader = ResourceManager::LoadShader("shaders/jeu.vs", "shaders/jeu.fs", nullptr, "jeu");
    ResourceManager::LoadShader("shaders/model.vs", "shaders/model.fs", nullptr, "model");
    ResourceManager::LoadShader("shaders/skybox.vs", "shaders/skybox.fs", nullptr, "skybox");
    // Configure shaders
    
    shader.Use().SetInteger("sprite", 0);
    // Load textures
    ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, GL_TRUE, "bg");
    ResourceManager::LoadTexture("textures/bones3.jpg", GL_FALSE, GL_TRUE, "sol");
    ResourceManager::LoadTexture("textures/fence.png", GL_TRUE, GL_FALSE, "fence");
    ResourceManager::LoadTexture("textures/dino1.png", GL_TRUE, GL_FALSE, "dino1");

    // Cubemap (Skybox)
    vector<const GLchar*> faces;
    faces.push_back("textures/skyboxes/mp_vod/rt.png");
    faces.push_back("textures/skyboxes/mp_vod/lf.png");
    faces.push_back("textures/skyboxes/mp_vod/up.png");
    faces.push_back("textures/skyboxes/mp_vod/dn.png");
    faces.push_back("textures/skyboxes/mp_vod/bk.png");
    faces.push_back("textures/skyboxes/mp_vod/ft.png");
    Texture3D skybox = ResourceManager::LoadCubemap(faces, "skybox");
    // Load levels
    GameLevel one = GameLevel(skybox);
    one.Load("levels/1.lvl");
    this->Levels.push_back(one);
    this->Level = 0;
    // Load models
    GameModel raptor = GameModel("models3d/Raptor/Raptor.obj", "raptor");
    raptor.Size = glm::vec3(0.2);
    raptor.Rotation=180;
    raptor.SetSide(0.5, DOWN_SIDE);
    this->Models.push_back(raptor);
    // Load sprites
    Sprite bg = Sprite(ResourceManager::GetTexture("bg"), this->Width, this->Height);
    std::vector<GLuint> n_max;
    n_max.push_back(6);
    n_max.push_back(8);
    n_max.push_back(6);
    n_max.push_back(7);
    n_max.push_back(1);
    n_max.push_back(1);
    n_max.push_back(1);
    Sprite dino1 = Sprite(ResourceManager::GetTexture("dino1"), this->Width, this->Height, n_max);
    dino1.Action=DEAD;
    this->Sprites.push_back(bg);
    this->Sprites.push_back(dino1);
    // Set render-specific controls
    Renderer3d = new SpriteRenderer(shader, GL_FALSE);
    RendererSkybox = new SpriteRenderer(ResourceManager::GetShader("skybox"), GL_TRUE);
    RendererBG = new SpriteRenderer(shader, bg.Tex_coord.x, bg.Tex_coord.y);
    RendererSprite = new SpriteRenderer(shader, dino1.Tex_coord.x, dino1.Tex_coord.y);
}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLfloat dt)
{
    // Camera controls
    if(this->Keys[GLFW_KEY_W])
        this->Cam.ProcessKeyboard(FORWARD, dt);
    if(this->Keys[GLFW_KEY_S])
        this->Cam.ProcessKeyboard(BACKWARD, dt);
    if(this->Keys[GLFW_KEY_A])
        this->Cam.ProcessKeyboard(LEFT, dt);
    if(this->Keys[GLFW_KEY_D])
        this->Cam.ProcessKeyboard(RIGHT, dt);
}
void Game::ProcessMouseMovement(GLdouble xpos, GLdouble ypos)
{
    if(this->firstMouse)
    {
        this->lastX = xpos;
        this->lastY = ypos;
        this->firstMouse = false;
    }

    GLfloat xoffset = xpos - this->lastX;
    GLfloat yoffset = this->lastY - ypos;  // Reversed since y-coordinates go from bottom to left
    
    this->lastX = xpos;
    this->lastY = ypos;

    this->Cam.ProcessMouseMovement(xoffset, yoffset);
}   


void Game::ProcessMouseScroll(GLdouble yoffset)
{
    this->Cam.ProcessMouseScroll(yoffset);
}


void Game::Render()
{    
	// Create camera transformation
    glm::mat4 view;
    view = this->Cam.GetViewMatrix();
    glm::mat4 projection3D = glm::perspective(glm::radians(this->Cam.Zoom), static_cast<GLfloat>(this->Width)/static_cast<GLfloat>(this->Height), 0.1f, 1000.0f);
    glm::mat4 projection2D = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);


    // Draw background
    //Renderer2d->DrawSprite(ResourceManager::GetTexture("bg"), glm::vec3(0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f,0.0f,1.0f), projection2D);
    //Renderer2d->DrawSprite(ResourceManager::GetTexture("sol"), glm::vec3(-(GLfloat)this->Width, -(GLfloat)(this->Height*3), -(GLfloat)(this->Height/2)), glm::vec2(this->Width*3, this->Height*3), 90.0f, glm::vec3(1.0f,0.0f,0.0f), projection3D, view);

    // Draw 2D
    this->Sprites[0].Draw(*RendererBG,projection2D);
    this->Sprites[1].Draw(*RendererSprite,projection2D);

    // // Draw models
    // this->Models[0].Draw(ResourceManager::GetShader("model"), projection3D, view);
    // // Draw level
    // this->Levels[this->Level].Draw(*Renderer3d, *RendererSkybox, projection3D, view);

}

