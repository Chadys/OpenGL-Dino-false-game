#include "game_object.h"
#include "resource_manager.h"


// Object2D::Object2D(Texture2D sprite, GLuint width, GLuint height)
//     : GameObject(sprite), Position(0), Size(glm::vec2(width,height)), Velocity(0), Reversed(GL_FALSE), Action(IDLE), N_img(0), Sprite_size(1) {
//         std::vector<GLuint> n_max;
//         n_max.push_back(1);
//         this->N_max=n_max;
//     }

Object2D::Object2D(Texture2D sprite, glm::vec2 position)
    : GameObject(sprite), Position(position), Reversed(GL_FALSE), Sprite_size(1), Alpha(1), Animation_timer(-1), Action(IDLE), N_img(0), Sound_played(GL_FALSE) {
        this->Size = glm::vec2(this->Tex.Width, this->Tex.Height);
        std::vector<GLuint> n_max;
        n_max.push_back(1);
        this->N_max=n_max;
    }

Object2D::Object2D(Texture2D sprite, glm::vec2 position, glm::vec2 size)
    : GameObject(sprite), Position(position), Size(size), Reversed(GL_FALSE), Sprite_size(1), Alpha(1), Animation_timer(-1), Action(IDLE), N_img(0), Sound_played(GL_FALSE) {
        std::vector<GLuint> n_max;
        n_max.push_back(1);
        this->N_max=n_max;
    }

Object2D::Object2D(Texture2D sprite, std::vector<GLuint> n_max, GLfloat size_factor, glm::vec2 position)
    : GameObject(sprite), Position(position), Reversed(GL_FALSE), Alpha(1), Animation_timer(-1), Action(IDLE), N_img(0), N_max(n_max), Sound_played(GL_FALSE) {
        GLuint max = 1;
        for (GLuint n : this->N_max)
            max = n > max ? n : max;
        this->Size=glm::vec2(sprite.Width/max,sprite.Height/this->N_max.size())*size_factor;
        this->Sprite_size.x=1.0f/max;
        this->Sprite_size.y=1.0f/this->N_max.size();
    }

void Object2D::Draw(StateManager &manager, SpriteRenderer &renderer, glm::mat4 projection, glm::mat4 view)
{
    renderer.DrawSprite(manager, this->Tex, this->Position, this->Size, GL_FALSE, projection, view, this->Alpha, this->Reversed, this->Action, (GLuint)this->N_img, this->Sprite_size);
}


void Object2D::Update(GLfloat dt){
    this->N_img += dt * (this->N_max[this->Action]-1)*2;
    if ((GLuint)this->N_img >= this->N_max[this->Action]){
        this->N_img = 0;
        if (this->Action == WHIP || this->Action == BITE)
            this->Action = IDLE;
    }
    if(this->Animation_timer > -1)
    	this->Animate(dt);
}

void Object2D::SetState(State state){
    if (this->Action != state){
        this->Action = state;
        this->N_img = 0;
    }
}

GLboolean Object2D::IsState(State state){
    if (this->Action == state)
        return GL_TRUE;
    return GL_FALSE;
}

glm::vec3 Object2D::ExchangeSprite(const Object2D &sprite2, const glm::vec3 &cam_pos){
	glm::vec3 new_cam_pos = glm::vec3(sprite2.Position-this->Position, 0.0f);
	this->Position += glm::vec2(cam_pos.x,cam_pos.y);
	return new_cam_pos;
}

// Play a dying animation
void Object2D::Animate(GLfloat dt){
	this->Animation_timer+=dt;
	if(this->Animation_timer > 3){
		if(this->Sound_played){
			this->SetState(DEAD);
			this->Alpha = 1;
			this->Sound_played = GL_FALSE;			
		}
		else
			this->Alpha -= dt/2;
		if (this->Alpha < 0){
			this->Alpha = 0;
			this->Animation_timer = -1;
		}
		return;
	}
	if(this->Animation_timer > 0.5){
        if (!this->Sound_played && !Mix_Playing(3)){
            Mix_PlayChannel(3, ResourceManager::GetSound("hurt"), 0);
            this->Sound_played = GL_TRUE;
        }
		this->SetState(HURT);
		this->Alpha -= dt*10;
		if (this->Alpha < 0)
			this->Alpha = 1;
	}
}