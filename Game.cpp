

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
#include <string>
#include <cstdlib>     /* srand, rand */
#include <ctime> 
#include <memory>


// Game-related State data
SpriteRenderer  *Renderer;
SpriteRenderer  *RendererSprite;
SpriteRenderer  *Renderer3d;
SpriteRenderer  *RendererSkybox;


Game::Game() 
	: State(GAME_MENU), Keys(), Cam(glm::vec3(0.0f, 0.0f, 0.0f)), lastX(400), lastY(300), firstMouse(true)
{ 
    this->Cam.MovementSpeed = 100.0f;
}

Game::~Game()
{
    delete Renderer;
    delete RendererSprite;
    delete Renderer3d;
    delete RendererSkybox;
}

void Game::Init()
{
    /* initialize random seed: */
    srand (time(NULL));

    // Load shaders
    Shader shader = ResourceManager::LoadShader("shaders/jeu.vs", "shaders/jeu.fs", nullptr, "jeu");
    ResourceManager::LoadShader("shaders/model.vs", "shaders/model.fs", nullptr, "model");
    ResourceManager::LoadShader("shaders/skybox.vs", "shaders/skybox.fs", nullptr, "skybox");
    // Configure shaders
    
    shader.Use().SetInteger("sprite", 0);
    // Load textures

    // TEXTURE 3D
    // ResourceManager::LoadTexture("textures/bones3.jpg", GL_FALSE, GL_TRUE, "sol");
    ResourceManager::LoadTexture("textures/blocks/fence.png", GL_TRUE, GL_FALSE, "fence");
   // SPRITE TEXTURE 
    ResourceManager::LoadTexture("textures/dino1.png", GL_TRUE, GL_FALSE, "dino1");
    ResourceManager::LoadTexture("textures/dino2.png", GL_TRUE, GL_FALSE, "dino2");
    //DECORS 2D
    std::unique_ptr<Tex> bg(new Texture2D(ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, GL_TRUE, "bg")));
    for (GLuint i = 0 ; i<6 ; i++)
            ResourceManager::LoadTexture(("textures/blocks/bdc_grass_side0"+to_string(i)+".png").c_str(), GL_TRUE, GL_TRUE, "grass"+to_string(i));
    ResourceManager::LoadTexture("textures/blocks/fern.png", GL_TRUE, GL_FALSE, "fern");
    ResourceManager::LoadTexture("textures/blocks/vine.png", GL_TRUE, GL_TRUE, "vine");
    // MENU
    std::unique_ptr<Tex> menu(new Texture2D(ResourceManager::LoadTexture("textures/menu.png", GL_TRUE, GL_TRUE, "menu")));
    ResourceManager::LoadTexture("textures/button-blue.png", GL_TRUE, GL_TRUE, "button");

    // Cubemap (Skybox)
    vector<const GLchar*> faces;
    faces.push_back("textures/skyboxes/mp_vod/rt.png");
    faces.push_back("textures/skyboxes/mp_vod/lf.png");
    faces.push_back("textures/skyboxes/mp_vod/up.png");
    faces.push_back("textures/skyboxes/mp_vod/dn.png");
    faces.push_back("textures/skyboxes/mp_vod/bk.png");
    faces.push_back("textures/skyboxes/mp_vod/ft.png");
    std::unique_ptr<Tex> skybox(new Texture3D(ResourceManager::LoadCubemap(faces, "skybox")));
    // Load levels
    this->Levels.push_back(GameLevel(menu));
    this->Levels[0].LoadMenu((GLfloat)this->Width, (GLfloat)this->Height);
    this->Level = 0;
    this->Levels.push_back(GameLevel(bg));
    this->Levels[1].Load("levels/1.lvl", this->Width, this->Height);
    this->Levels.push_back(GameLevel(skybox));
    this->Levels[2].Load("levels/2.lvl");
    // Load models
    GameModel raptor = GameModel("models3d/Raptor/Raptor.obj", "raptor");
    raptor.Size = glm::vec3(0.2);
    raptor.Rotation=180;
    raptor.SetSide(0.5, DOWN_SIDE);
    this->Models.push_back(raptor);
    // Load sprites
    std::vector<GLuint> n_max;
    n_max.push_back(6);
    n_max.push_back(8);
    n_max.push_back(6);
    n_max.push_back(7);
    n_max.push_back(1);
    n_max.push_back(1);
    n_max.push_back(1);
    n_max.push_back(0);
    Object2D dino1 = Object2D(ResourceManager::GetTexture("dino1"), n_max);
    dino1.Position = glm::vec2(this->Width/2-dino1.Size.x/2,this->Height/2-dino1.Size.y-ResourceManager::GetTexture("grass0").Height/2+2); // the +2 is here because the dinosaur's paw aren't at the same height
    dino1.Reversed = GL_TRUE;
    this->Sprites.push_back(dino1);
    Object2D dino2 = Object2D(ResourceManager::GetTexture("dino2"), n_max);
    dino2.Position = glm::vec2(this->Width/2-dino2.Size.x/2 + ResourceManager::GetTexture("grass0").Width*6, this->Height/2-dino2.Size.y-ResourceManager::GetTexture("grass0").Height/2+2); // the +2 is here because the dinosaur's paw aren't at the same height
    this->Sprites.push_back(dino2);
    // Set render-specific controls
    Renderer3d = new SpriteRenderer(shader, GL_FALSE);
    RendererSkybox = new SpriteRenderer(ResourceManager::GetShader("skybox"), GL_TRUE);
    Renderer = new SpriteRenderer(shader, 1, 1);
    RendererSprite = new SpriteRenderer(shader, dino1.Sprite_size.x, dino1.Sprite_size.y);
}

void Game::Update(GLfloat dt)
{
    for (Object2D &sprite : Sprites)
        sprite.Update(dt);
}


void Game::ProcessInput(GLfloat dt)
{
    GLboolean move = GL_FALSE;

    // Camera controls
    if(this->State == GAME_3D){
        if(this->Keys[GLFW_KEY_S])
            this->Cam.ProcessKeyboard(BACKWARD, dt);
        if(this->Keys[GLFW_KEY_A])
            this->Cam.ProcessKeyboard(LEFT, dt);
        if(this->Keys[GLFW_KEY_D])
            this->Cam.ProcessKeyboard(RIGHT, dt);
        if(this->Keys[GLFW_KEY_W])
            this->Cam.ProcessKeyboard(FORWARD, dt);

        if(this->Keys[GLFW_KEY_ENTER])
            this->Go2D();
    }
    else{
        if(this->Keys[GLFW_KEY_S]){
            this->Cam.ProcessKeyboard(DOWN, dt);
            this->Sprites[0].SetState(DEAD);
            move = GL_TRUE;
        }
        if(this->Keys[GLFW_KEY_A]){
            this->Cam.ProcessKeyboard(LEFT, dt);
            this->Sprites[0].Reversed = GL_FALSE;
            this->Sprites[0].SetState(WALK);
            move = GL_TRUE;
        }
        if(this->Keys[GLFW_KEY_D]){
            this->Cam.ProcessKeyboard(RIGHT, dt);
            this->Sprites[0].Reversed = GL_TRUE;
            this->Sprites[0].SetState(WALK);
            move = GL_TRUE;
        }
        if(this->Keys[GLFW_KEY_W]){
            this->Cam.ProcessKeyboard(UP, dt);
            this->Sprites[0].SetState(JUMP);
            move = GL_TRUE;
        }

        if(this->Keys[GLFW_KEY_SPACE])
            this->Sprites[0].SetState(WHIP);
        if(this->Keys[GLFW_KEY_RIGHT_SHIFT])
            this->Sprites[1].SetState(BITE);

        if (!move && !this->Sprites[0].IsState(WHIP))
            this->Sprites[0].SetState(IDLE);

        if(this->Keys[GLFW_KEY_ENTER])
            this->Go3D();
    }
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
    glm::mat4 view3D, view2D, projection3D, projection2D;
    view3D = this->Cam.GetViewMatrix();
    view2D = this->Cam.GetViewMatrix2D();
    projection3D = glm::perspective(glm::radians(this->Cam.Zoom), static_cast<GLfloat>(this->Width)/static_cast<GLfloat>(this->Height), 0.1f, 1000.0f);
    projection2D = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

    // Draw background
    //Renderer2d->DrawSprite(ResourceManager::GetTexture("bg"), glm::vec3(0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f,0.0f,1.0f), projection2D);
    //Renderer2d->DrawSprite(ResourceManager::GetTexture("sol"), glm::vec3(-(GLfloat)this->Width, -(GLfloat)(this->Height*3), -(GLfloat)(this->Height/2)), glm::vec2(this->Width*3, this->Height*3), 90.0f, glm::vec3(1.0f,0.0f,0.0f), projection3D, view);
    switch(this->State){
        case (GAME_MENU):
            this->Levels[0].Draw(*Renderer, this->Width, this->Height, projection2D, glm::mat4(), GL_TRUE);
            break;
        case(GAME_2D):
            // Draw level
            this->Levels[1].Draw(*Renderer, this->Width, this->Height, projection2D, view2D);
            // Draw sprites
            this->Sprites[0].Draw(*RendererSprite,projection2D);
            this->Sprites[1].Draw(*RendererSprite,projection2D, view2D);
            break;
        default:
            // Draw models
            this->Models[0].Draw(ResourceManager::GetShader("model"), projection3D, view3D);
            // Draw level
            this->Levels[2].Draw(*Renderer3d, *RendererSkybox, projection3D, view3D);
    }
}

void Game::Go2D(){
    this->State = GAME_2D;
    this->Cam = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); 
    this->Cam.MovementSpeed = 100.0f;
}
void Game::Go3D(){
    this->State = GAME_3D;
    this->Cam = Camera(glm::vec3(0.0f, 0.5-this->Models[0].model.Span_udb.y*this->Models[0].Size.y, 10.0f));
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); 
    this->Cam.MovementSpeed = SPEED;
}















// vec3 beziertest(vec3 depart,vec3 middle,vec3 arrivee,float t){
//     vec3 bob;
//     bob[0]=(1-t)*(1-t)*depart[0]+2*t*(1-t)*middle[0]+t*t*arrivee[0];
//     bob[1]=(1-t)*(1-t)*depart[1]+2*t*(1-t)*middle[1]+t*t*arrivee[1];
//     bob[2]=(1-t)*(1-t)*depart[2]+2*t*(1-t)*middle[2]+t*t*arrivee[2];
//     return bob;
// }
