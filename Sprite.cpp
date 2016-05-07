#include "sprite.h"


Sprite::Sprite(Texture2D sprite, GLuint width, GLuint height)
    : Position(0), Size(glm::vec2(width,height)), Velocity(0), Reversed(GL_FALSE), Action(IDLE), N_img(0), Sprite_size(1) {
        this->Sheet=sprite;
        std::vector<GLuint> n_max;
        n_max.push_back(1);
        this->N_max=n_max;
    }

Sprite::Sprite(Texture2D sprite, std::vector<GLuint> n_max)
    : Position(0), Velocity(0), Reversed(GL_FALSE), Action(IDLE), N_img(0), N_max(n_max) {
        this->Sheet=sprite;
        GLuint max = 1;
        for (GLuint n : this->N_max)
            max = n > max ? n : max;
        this->Size=glm::vec2(sprite.Width/max,sprite.Height/this->N_max.size());
        this->Sprite_size.x=1.0f/max;
        this->Sprite_size.y=1.0f/this->N_max.size();
    }

void Sprite::Draw(SpriteRenderer &renderer, glm::mat4 projection, glm::mat4 view)
{
    renderer.DrawSprite(this->Sheet, this->Position, this->Size, this->Reversed, this->Action, this->N_img, this->Sprite_size, projection, view);
}