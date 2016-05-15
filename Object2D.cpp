#include "game_object.h"


// Object2D::Object2D(Texture2D sprite, GLuint width, GLuint height)
//     : GameObject(sprite), Position(0), Size(glm::vec2(width,height)), Velocity(0), Reversed(GL_FALSE), Action(IDLE), N_img(0), Sprite_size(1) {
//         std::vector<GLuint> n_max;
//         n_max.push_back(1);
//         this->N_max=n_max;
//     }

Object2D::Object2D(Texture2D sprite, glm::vec2 position)
    : GameObject(sprite), Position(position), Reversed(GL_FALSE), Sprite_size(1), Action(IDLE), N_img(0) {
        this->Size = glm::vec2(this->Tex.Width, this->Tex.Height);
        std::vector<GLuint> n_max;
        n_max.push_back(1);
        this->N_max=n_max;
    }

Object2D::Object2D(Texture2D sprite, glm::vec2 position, glm::vec2 size)
    : GameObject(sprite), Position(position), Size(size), Reversed(GL_FALSE), Sprite_size(1), Action(IDLE), N_img(0) {
        std::vector<GLuint> n_max;
        n_max.push_back(1);
        this->N_max=n_max;
    }

Object2D::Object2D(Texture2D sprite, std::vector<GLuint> n_max, GLfloat size_factor, glm::vec2 position)
    : GameObject(sprite), Position(position), Reversed(GL_FALSE), Action(IDLE), N_img(0), N_max(n_max) {
        GLuint max = 1;
        for (GLuint n : this->N_max)
            max = n > max ? n : max;
        this->Size=glm::vec2(sprite.Width/max,sprite.Height/this->N_max.size())*size_factor;
        this->Sprite_size.x=1.0f/max;
        this->Sprite_size.y=1.0f/this->N_max.size();
    }

void Object2D::Draw(SpriteRenderer &renderer, glm::mat4 projection, glm::mat4 view)
{
    renderer.DrawSprite(this->Tex, this->Position, this->Size, GL_FALSE, projection, view, this->Reversed, this->Action, (GLuint)this->N_img, this->Sprite_size);
}


void Object2D::Update(GLfloat dt){
    this->N_img += dt * (this->N_max[this->Action]-1)*2;
    if ((GLuint)this->N_img >= this->N_max[this->Action]){
        this->N_img = 0;
        if (this->Action == WHIP || this->Action == BITE)
            this->Action = IDLE;
    }
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