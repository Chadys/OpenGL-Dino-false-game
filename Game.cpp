/*******************************************************************
** Taken and adapted from learnopengl.com (part of a Breakout game) 
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <ctime> 
#include <memory>


// Game-related State data
SpriteRenderer  *Renderer;
SpriteRenderer  *RendererSprite;
SpriteRenderer  *Renderer3d;
SpriteRenderer  *RendererSkybox;

/*------------------------------------CONSTRUCTOR/DESTRUCTOR-----------------------------------------*/
Game::Game() 
	: State(GAME_MENU), Keys(), ProcessedKeys(), Cam(glm::vec3(0.0f, 0.0f, 0.0f)), lastX(400), lastY(300), firstMouse(true), Selected_sprite(GL_FALSE), Music("chaosring"), DinoSpin(0.0f), Change_level(GL_FALSE), Turbo(GL_FALSE) {
		this->bezier.target = NOP;
	}

Game::~Game()
{
    delete Renderer;
    delete RendererSprite;
    delete Renderer3d;
    delete RendererSkybox;
}

/*------------------------------------INIT-----------------------------------------*/
void Game::Init()
{
	//Adapt camera speed for 2D and depending on the screen size (it was originalled choosen for a 800x600 screen)
    this->Cam.MovementSpeed = 100.0f*this->Width/800;

    // Load shaders
    Shader shader = ResourceManager::LoadShader("shaders/jeu.vs", "shaders/jeu.fs", nullptr, "jeu");
    ResourceManager::LoadShader("shaders/model.vs", "shaders/model.fs", nullptr, "model");
    ResourceManager::LoadShader("shaders/skybox.vs", "shaders/skybox.fs", nullptr, "skybox");
    ResourceManager::LoadShader("shaders/text.vs", "shaders/text.fs", nullptr, "text");
    // Configure shaders
    shader.Use().SetInteger("sprite", 0);

    //Load musics
    ResourceManager::LoadMusic("sounds/Chaos Rings 2_ It's Me We're Talkin' About! - Noriyasu Agematsu.mp3", "chaosring");
    ResourceManager::LoadMusic("sounds/Jurassic Park Game Boy Level 1 Music.mp3", "jurassic");
    ResourceManager::LoadMusic("sounds/Mega Man 2_ Dr. Wily Stage 1 Music EXTENDED.mp3", "megaman");
    ResourceManager::LoadMusic("sounds/Techno Trance - Tetris.mp3", "tetris");
    Mix_VolumeMusic(MIX_MAX_VOLUME/1.5);

    //Load sounds
    ResourceManager::LoadSound("sounds/blop.wav", "jump");
    ResourceManager::LoadSound("sounds/button.wav", "button");
    ResourceManager::LoadSound("sounds/dino_hurt.wav", "hurt");
    ResourceManager::LoadSound("sounds/dino_scream.wav", "growl");
    ResourceManager::LoadSound("sounds/quit.wav", "quit");
    ResourceManager::LoadSound("sounds/scratch.wav", "scratch");
    ResourceManager::LoadSound("sounds/whip.wav", "whip");
    ResourceManager::LoadSound("sounds/helice.wav", "helice");
    ResourceManager::LoadSound("sounds/blip.wav", "switch");
    Mix_AllocateChannels(4);
    Mix_Volume(1, MIX_MAX_VOLUME/3);
    Mix_Volume(4, MIX_MAX_VOLUME);

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
    std::pair<std::vector<glm::vec3>,std::vector<glm::vec3>> cubes_carac = this->Levels[2].Load("levels/2.lvl");
    // Load models
    GameModel raptor = GameModel("models3d/Raptor/Raptor.obj", "raptor");
    raptor.Size = glm::vec3(0.2);
    raptor.Rotation.y=180;
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
    Object2D dino1 = Object2D(ResourceManager::GetTexture("dino1"), n_max, (GLfloat)this->Width/1024);
    dino1.Position = glm::vec2(this->Width/2-dino1.Size.x/2,this->Height/2-dino1.Size.y/2+2); // the +2 is here because the dinosaur's paw aren't at the same height
    dino1.Reversed = GL_TRUE;
    this->Sprites.push_back(dino1);
    Object2D dino2 = Object2D(ResourceManager::GetTexture("dino2"), n_max, (GLfloat)this->Width/1024, glm::vec2(dino1.Position.x + ResourceManager::GetTexture("grass0").Width*41*((GLfloat)this->Width/1024), dino1.Position.y - ResourceManager::GetTexture("grass0").Height*2*((GLfloat)this->Width/1024)));
    this->Sprites.push_back(dino2);
    // Set render-specific controls
    Renderer3d = new SpriteRenderer(shader, cubes_carac);
    RendererSkybox = new SpriteRenderer(ResourceManager::GetShader("skybox"));
    Renderer = new SpriteRenderer(shader, 1, 1);
    RendererSprite = new SpriteRenderer(shader, dino1.Sprite_size.x, dino1.Sprite_size.y);
}

/*------------------------------------UPDATE-----------------------------------------*/
void Game::Update(GLfloat dt)
{
    this->State_manager.Update(dt);

    if (!Mix_PlayingMusic()){
        Mix_FadeInMusic(ResourceManager::GetMusic(this->Music), -1, 0);
        if (!this->Music.compare("chaosring")){
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE); 
        }
        else if (!this->Music.compare("tetris")){
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE); 
        }
    }

    this->Models[0].Update(dt, this->DinoSpin);

    if (this->State == GAME_2D){
        for (Object2D &sprite : Sprites)
            sprite.Update(dt);
        this->Levels[1].Update(dt, this->Width, this->Height, glm::vec2(this->Cam.Position.x,this->Cam.Position.y));
    }

    if (this->State == GAME_3D && this->Turbo){
        this->Cam.Position.x += dt * (this->DinoSpin/25);
        this->Models[0].Position.x += dt * (this->DinoSpin/25);
        if (this->Models[0].Position.x > 1000){
            this->Cam.Position.x += -1500-this->Models[0].Position.x;
            this->Models[0].Position.x = -1500;
        }
    }


    if(this->bezier.target == CAM_JUMP){
    	float dist = sqrt(pow(this->bezier.depart.x-this->bezier.arrivee.x,2)+pow(this->bezier.depart.y-this->bezier.arrivee.y,2)+pow(this->bezier.depart.z-this->bezier.arrivee.z,2));
    	this->bezier.time_elapsed += (1/dist*500)*dt;
    	this->Cam.Position = Game::Update_Bezier(this->bezier.depart,this->bezier.middle,this->bezier.arrivee,this->bezier.time_elapsed);
    	if (this->bezier.time_elapsed >= 1)
    		this->bezier.target = NOP;
    }
    if(this->bezier.target == CAM_SLIDE){
    	this->bezier.time_elapsed += dt;
    	this->Cam.Position = Game::Update_Bezier(this->bezier.depart,this->bezier.middle,this->bezier.arrivee,this->bezier.time_elapsed);
    	if (this->bezier.time_elapsed >= 1){
    		this->bezier.target = NOP;
    		this->Sprites[Selected_sprite].Position = glm::vec2(this->Width/2-this->Sprites[Selected_sprite].Size.x/2,this->Height/2-this->Sprites[Selected_sprite].Size.y/2+2);
    	}
    }
}

/*------------------------------------PROCESSORS-----------------------------------------*/
void Game::ProcessInput(GLfloat dt)
{

    if(this->Keys[GLFW_KEY_ENTER] && !this->ProcessedKeys[GLFW_KEY_ENTER]){
        this->ProcessedKeys[GLFW_KEY_ENTER] = GL_TRUE;
        if (this->State == GAME_MENU)
            Mix_PlayChannel(0, ResourceManager::GetSound("button"), 0);
        else
            Change_level = GL_TRUE;
        this->State_manager.Fade = FADOUT;
        Mix_FadeOutMusic(1000);
    }
    if(this->Keys[GLFW_KEY_ESCAPE]){
        Mix_PlayChannel(0, ResourceManager::GetSound("quit"), 0);
        while(Mix_Playing(0));
    }

    if(this->State == GAME_3D){
        // Cam
        if(this->Keys[GLFW_KEY_S])
            this->Cam.ProcessKeyboard(BACKWARD, dt);
        if(this->Keys[GLFW_KEY_A])
            this->Cam.ProcessKeyboard(LEFT, dt);
        if(this->Keys[GLFW_KEY_D])
            this->Cam.ProcessKeyboard(RIGHT, dt);
        if(this->Keys[GLFW_KEY_W])
            this->Cam.ProcessKeyboard(FORWARD, dt);
        // Dino rotation and color
        if(this->Keys[GLFW_KEY_KP_ADD]){
            GLfloat new_spin = this->DinoSpin + 100*dt;
            this->ChangeVolume(this->DinoSpin/10, new_spin/10);
            this->DinoSpin = new_spin;
            if (!Mix_Playing(2))
                Mix_PlayChannel(2, ResourceManager::GetSound("helice"), -1);
        }
        if(this->Keys[GLFW_KEY_KP_SUBTRACT]){
            GLfloat new_spin = this->DinoSpin - 100*dt;
            if (new_spin < 0)
                new_spin = 0;
            this->ChangeVolume(this->DinoSpin/10, new_spin/10);
            this->DinoSpin = new_spin;
            if (!Mix_Playing(2))
                Mix_PlayChannel(2, ResourceManager::GetSound("helice"), -1);

        }
        // Turbo
        if(this->Keys[GLFW_KEY_SPACE] && !this->ProcessedKeys[GLFW_KEY_SPACE]){
            this->ProcessedKeys[GLFW_KEY_SPACE] = GL_TRUE;
            this->Turbo = !this->Turbo;
        }
    }

    else if(this->State == GAME_2D){
    	GLboolean move = GL_FALSE, walk = GL_FALSE;
    	if (this->bezier.target == CAM_JUMP){
    		this->Sprites[this->Selected_sprite].SetState(JUMP);
    		return;
    	}
        if (this->bezier.target == CAM_SLIDE){
            this->Sprites[this->Selected_sprite].SetState(IDLE);
            return;
        }
        if(this->Keys[GLFW_KEY_BACKSPACE] && !this->ProcessedKeys[GLFW_KEY_BACKSPACE]){
        	this->ProcessedKeys[GLFW_KEY_BACKSPACE] = GL_TRUE;
        	this->SetBezier(this->Sprites[this->Selected_sprite].ExchangeSprite(this->Sprites[!Selected_sprite], this->Cam.Position));
        	this->Sprites[this->Selected_sprite].SetState(IDLE);
            this->Selected_sprite = !this->Selected_sprite;
        } 
        // --------------------------------JUMPS --------------------------------- //
        if(this->Keys[GLFW_KEY_KP_0] && !this->ProcessedKeys[GLFW_KEY_KP_0]){
        	this->ProcessedKeys[GLFW_KEY_KP_0] = GL_TRUE;
            Mix_PlayChannel(0, ResourceManager::GetSound("jump"), 0);
            this->SetBezier(2, 0);
        }
        if(this->Keys[GLFW_KEY_KP_1] && !this->ProcessedKeys[GLFW_KEY_KP_1]){
        	this->ProcessedKeys[GLFW_KEY_KP_1] = GL_TRUE;
            Mix_PlayChannel(0, ResourceManager::GetSound("jump"), 0);
            this->SetBezier(1, 1);
        }
        if(this->Keys[GLFW_KEY_KP_2] && !this->ProcessedKeys[GLFW_KEY_KP_2]){
        	this->ProcessedKeys[GLFW_KEY_KP_2] = GL_TRUE;
            Mix_PlayChannel(0, ResourceManager::GetSound("jump"), 0);
            this->SetBezier(1, -1);
        }
        if(this->Keys[GLFW_KEY_KP_3] && !this->ProcessedKeys[GLFW_KEY_KP_3]){
        	this->ProcessedKeys[GLFW_KEY_KP_3] = GL_TRUE;
            Mix_PlayChannel(0, ResourceManager::GetSound("jump"), 0);
            this->SetBezier(2, -1);
        }
        if(this->Keys[GLFW_KEY_KP_4] && !this->ProcessedKeys[GLFW_KEY_KP_4]){
        	this->ProcessedKeys[GLFW_KEY_KP_4] = GL_TRUE;
            Mix_PlayChannel(0, ResourceManager::GetSound("jump"), 0);
            this->SetBezier(3, -2);
        }
        if(this->Keys[GLFW_KEY_KP_5] && !this->ProcessedKeys[GLFW_KEY_KP_5]){
        	this->ProcessedKeys[GLFW_KEY_KP_5] = GL_TRUE;
            Mix_PlayChannel(0, ResourceManager::GetSound("jump"), 0);
            this->SetBezier(6, -2);
        }
        if(this->Keys[GLFW_KEY_KP_6] && !this->ProcessedKeys[GLFW_KEY_KP_6]){
        	this->ProcessedKeys[GLFW_KEY_KP_6] = GL_TRUE;
            Mix_PlayChannel(0, ResourceManager::GetSound("jump"), 0);
            this->SetBezier(3, -1);
        }
        if(this->Keys[GLFW_KEY_KP_7] && !this->ProcessedKeys[GLFW_KEY_KP_7]){
        	this->ProcessedKeys[GLFW_KEY_KP_7] = GL_TRUE;
            Mix_PlayChannel(0, ResourceManager::GetSound("jump"), 0);
            this->SetBezier(6, -5);
        }
        // ------------------------------------------------------------------- //

        if(this->Keys[GLFW_KEY_KP_ADD] && !this->ProcessedKeys[GLFW_KEY_KP_ADD]){
            this->ProcessedKeys[GLFW_KEY_KP_ADD] = GL_TRUE;
            this->Levels[1].AddCircles(this->Width, this->Height, glm::vec2(this->Cam.Position.x,this->Cam.Position.y));
        }
        if(this->Keys[GLFW_KEY_S]){
            this->Cam.ProcessKeyboard(DOWN, dt);
            this->Sprites[Selected_sprite].SetState(DEAD);
            move = GL_TRUE;
        }
        if(this->Keys[GLFW_KEY_W]){
            this->Cam.ProcessKeyboard(UP, dt);
            this->Sprites[Selected_sprite].SetState(JUMP);
            move = GL_TRUE;
        }
        if(this->Keys[GLFW_KEY_A]){
            this->Cam.ProcessKeyboard(LEFT, dt);
            this->Sprites[Selected_sprite].Reversed = GL_FALSE;
            this->Sprites[Selected_sprite].SetState(WALK);
            if(!Mix_Playing(1) && !move)
                Mix_PlayChannel(1, ResourceManager::GetSound("scratch"), -1);
            move = GL_TRUE;
            walk = GL_TRUE;
        }
        if(this->Keys[GLFW_KEY_D]){
            this->Cam.ProcessKeyboard(RIGHT, dt);
            this->Sprites[Selected_sprite].Reversed = GL_TRUE;
            this->Sprites[Selected_sprite].SetState(WALK);
            if(!Mix_Playing(1) && !move)
                Mix_PlayChannel(1, ResourceManager::GetSound("scratch"), -1);
            move = GL_TRUE;
            walk = GL_TRUE;
        }

        if(this->Keys[GLFW_KEY_SPACE] && !this->ProcessedKeys[GLFW_KEY_SPACE]){
            this->ProcessedKeys[GLFW_KEY_SPACE] = GL_TRUE;
            this->Sprites[Selected_sprite].SetState(WHIP);
            Mix_PlayChannel(0, ResourceManager::GetSound("whip"), 0);
        }
        if(this->Keys[GLFW_KEY_RIGHT_SHIFT] && !this->ProcessedKeys[GLFW_KEY_RIGHT_SHIFT]){
            this->ProcessedKeys[GLFW_KEY_RIGHT_SHIFT] = GL_TRUE;
            this->Sprites[!Selected_sprite].SetState(BITE);
            //Start hurt animation on the other dino
            this->Sprites[Selected_sprite].Animation_timer = 0;
            this->Music = "megaman";
            Mix_PlayChannel(0, ResourceManager::GetSound("growl"), 0);
        }

        if (!move && !this->Sprites[Selected_sprite].IsState(WHIP) && !this->Sprites[Selected_sprite].IsState(DEAD))
            this->Sprites[Selected_sprite].SetState(IDLE);

        if(Mix_Playing(1) && !walk)
            Mix_HaltChannel(1);
    }

    // Menu
    else{
        if(this->Keys[GLFW_KEY_UP] && !this->ProcessedKeys[GLFW_KEY_UP]){
        	this->ProcessedKeys[GLFW_KEY_UP] = GL_TRUE;
        	this->Levels[0].ChangeButton(GL_TRUE);
            Mix_PlayChannel(0, ResourceManager::GetSound("switch"), 0);
        }
        if(this->Keys[GLFW_KEY_DOWN] && !this->ProcessedKeys[GLFW_KEY_DOWN]){
        	this->ProcessedKeys[GLFW_KEY_DOWN] = GL_TRUE;
        	this->Levels[0].ChangeButton(GL_FALSE);
            Mix_PlayChannel(0, ResourceManager::GetSound("switch"), 0);
        }
    }
}

void Game::ProcessMouseMovement(GLdouble xpos, GLdouble ypos)
{
	if(this->State == GAME_3D){
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
}   


void Game::ProcessMouseScroll(GLdouble yoffset)
{
	if(this->State == GAME_3D)
    	this->Cam.ProcessMouseScroll(yoffset);
}
void Game::ProcessEndingMusic(){
    Mix_HaltChannel(-1);
    switch(this->State){
        case (GAME_MENU):
            this->Go2D();
            break;
        case(GAME_2D):
            if (Change_level)
                this->Go3D();
            else
                Change_level = GL_TRUE;
            break;
        default: // 3D
            this->GoMENU();
    }
}

/*------------------------------------RENDER-----------------------------------------*/
void Game::Render()
{    
	// Create camera transformation
    glm::mat4 view3D, view2D, projection3D, projection2D;
    view3D = this->Cam.GetViewMatrix();
    // view2D = this->Cam.GetViewMatrix2D();
    projection3D = glm::perspective(glm::radians(this->Cam.Zoom), static_cast<GLfloat>(this->Width)/static_cast<GLfloat>(this->Height), 0.1f, 1000.0f);
    projection2D = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

    // Draw background
    //Renderer2d->DrawSprite(ResourceManager::GetTexture("bg"), glm::vec3(0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f,0.0f,1.0f), projection2D);
    //Renderer2d->DrawSprite(ResourceManager::GetTexture("sol"), glm::vec3(-(GLfloat)this->Width, -(GLfloat)(this->Height*3), -(GLfloat)(this->Height/2)), glm::vec2(this->Width*3, this->Height*3), 90.0f, glm::vec3(1.0f,0.0f,0.0f), projection3D, view);
    switch(this->State){
        case (GAME_MENU):
            this->Levels[0].Draw(this->State_manager, *Renderer, this->Width, this->Height, projection2D, glm::mat4(), GL_TRUE);
            break;
        case(GAME_2D):
            // Draw level
            this->Levels[1].Draw(this->State_manager, *Renderer, this->Width, this->Height, projection2D, view3D);
            // Draw sprites
            this->Sprites[!Selected_sprite].Draw(this->State_manager, *RendererSprite,projection2D, view3D);
            if(this->bezier.target == CAM_SLIDE)
            	this->Sprites[Selected_sprite].Draw(this->State_manager, *RendererSprite,projection2D, view3D);
            else
            	this->Sprites[Selected_sprite].Draw(this->State_manager, *RendererSprite,projection2D);
            break;
        default: // 3D
            // Draw models
            this->Models[0].Draw(this->State_manager, ResourceManager::GetShader("model"), projection3D, view3D);
            // Draw level
            this->Levels[2].Draw(this->State_manager, *Renderer3d, *RendererSkybox, projection3D, view3D);
    }
}

/*------------------------------------CHANGE_STATE-----------------------------------------*/
void Game::GoMENU(){
    this->Music = "chaosring";
    this->State = GAME_MENU;
    this->Cam = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    this->Cam.MovementSpeed = 100.0f*this->Width/800;
}
void Game::Go2D(){
    this->Music = "jurassic";
    this->State = GAME_2D;
    this->Levels[0].ActiveContinue();
}
void Game::Go3D(){
    this->Music = "tetris";
    this->State = GAME_3D;
    this->Cam = Camera(glm::vec3(0.0f, 0.5-this->Models[0].model.Span_udb.y*this->Models[0].Size.y, 10.0f));
    this->Cam.MovementSpeed = SPEED;
    this->DinoSpin = 0.0f;
    Mix_Volume(2, 0);
}

/*------------------------------------MISCELLANOUS-----------------------------------------*/
glm::vec3 Game::Update_Bezier(glm::vec3 depart,glm::vec3 middle,glm::vec3 arrivee, GLfloat t){
	glm::vec3 new_vec;
	if (t > 1)
		t = 1;
   	new_vec[0]=(1-t)*(1-t)*depart[0]+2*t*(1-t)*middle[0]+t*t*arrivee[0];
   	new_vec[1]=(1-t)*(1-t)*depart[1]+2*t*(1-t)*middle[1]+t*t*arrivee[1];
   	new_vec[2]=(1-t)*(1-t)*depart[2]+2*t*(1-t)*middle[2]+t*t*arrivee[2];
   	return new_vec;
}

void Game::SetBezier(GLint x, GLint y){
	this->bezier.target = CAM_JUMP;
	this->bezier.time_elapsed = 0.0f;
	this->bezier.depart = this->Cam.Position;
	this->bezier.arrivee = glm::vec3(this->Cam.Position.x+ResourceManager::GetTexture("grass0").Width*((GLfloat)this->Width/1024)*x, this->Cam.Position.y-ResourceManager::GetTexture("grass0").Height*((GLfloat)this->Width/1024)*y, this->Cam.Position.z);
	GLfloat h_factor = y < 0 ? -y : 1.0f;
	this->bezier.middle = glm::vec3(this->bezier.depart.x+(this->bezier.arrivee.x-this->bezier.depart.x)/2, this->bezier.depart.y+(this->bezier.arrivee.y-this->bezier.depart.y)/2-150*((GLfloat)this->Width/1024)*h_factor, 0.0f);
}

void Game::SetBezier(glm::vec3 new_cam_pos){
	this->bezier.target = CAM_SLIDE;
	this->bezier.time_elapsed = 0.0f;
	this->bezier.depart = this->Cam.Position;
	this->bezier.arrivee = new_cam_pos;
	this->bezier.middle = this->bezier.depart+(this->bezier.arrivee-this->bezier.depart)*0.5f;
}

// Change the volume of the helice sound if the raptor's rotation speed change.
void Game::ChangeVolume(GLfloat prev, GLfloat next){
    GLuint etape = 15;
    GLfloat step = MIX_MAX_VOLUME/etape;
    if ((next >= MIX_MAX_VOLUME && prev < MIX_MAX_VOLUME) || (next == 0 && prev > 0)){
        Mix_Volume(2, next);
        Mix_HaltChannel(2);
        return;
    }

    for (GLuint i = 0 ; i < etape ; i++){
        GLuint volume = i*step;
        if(prev < volume && next > volume){
            Mix_Volume(2, volume);
            Mix_HaltChannel(2);
        }
    }
}