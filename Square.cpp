#include "game_object.h"


Square::Square(glm::vec2 position, glm::vec2 size, glm::vec3 color1, glm::vec3 color2, Effect effect, glm::vec2 velocity, GLboolean circle)
    : Position(position), Size(size), Velocity(velocity), IsCircle(circle), Color1(color1), Color2(color2), Effet(effect) { }

void Square::Draw(StateManager &manager, SpriteRenderer &renderer, glm::mat4 projection, glm::mat4 view, GLboolean border)
{
    renderer.DrawSprite(manager, this->Position, this->Size, this->IsCircle, this->Color1, this->Color2, this->Effet, projection, view, border);
}


void Square::Update(GLfloat dt){
    this->Position += this->Velocity * dt;
}