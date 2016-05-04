

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_object.h"


GameObject::GameObject() 
    : Position(0, 0, 0), Size(1, 1, 1), Velocity(0.0f), Color(1.0f), Alpha(1.0f), Rotation(0.0f), Rotation_angle(0.0f, 0.0f, 1.0f), Sprite() { }

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity) 
    : Position(pos), Size(size), Velocity(velocity), Color(color), Alpha(1.0f), Rotation(0.0f), Rotation_angle(0.0f, 0.0f, 1.0f), Sprite(sprite) { }

void GameObject::Draw(SpriteRenderer &renderer, glm::mat4 projection, glm::mat4 view)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Rotation_angle, this->Color, this->Alpha, projection, view);
}

