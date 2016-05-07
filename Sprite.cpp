#include "sprite.h"


Sprite::Sprite(Texture2D sprite, GLuint game_width, GLuint game_height)
    : Position(0), Size(glm::vec2(game_width,game_height)), Velocity(0), Reversed(GL_FALSE), Action(IDLE), N_img(0), Steps(1), Tex_coord(1) {
        this->Sheet=sprite;
        std::vector<GLuint> n_max;
        n_max.push_back(1);
        this->N_max=n_max;
    }

Sprite::Sprite(Texture2D sprite, GLuint game_width, GLuint game_height, std::vector<GLuint> n_max)
    : Position(0), Velocity(0), Reversed(GL_FALSE), Action(IDLE), N_img(0), N_max(n_max) {
        this->Sheet=sprite;
        GLuint max = 1;
        for (GLuint n : this->N_max)
            max = n > max ? n : max;
        this->Size=glm::vec2(sprite.Width/max,sprite.Height/N_max.size());
        this->Tex_coord.x=((GLfloat)this->Size.y)/sprite.Height;
        this->Tex_coord.y=((GLfloat)this->Size.x)/sprite.Width;
        this->Steps.x=((GLfloat)this->Size.y)/game_height;
        this->Steps.y=((GLfloat)this->Size.x)/game_width;
    }

void Sprite::Draw(SpriteRenderer &renderer, glm::mat4 projection, glm::mat4 view)
{
    renderer.DrawSprite(this->Sheet, this->Position, this->Size, this->Reversed, this->Action, this->N_img, this->Steps, projection, view);
}