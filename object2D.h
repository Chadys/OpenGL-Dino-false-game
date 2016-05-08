#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "texture.h"
#include "sprite_renderer.h"

enum State {
    IDLE,
    WALK,
    BITE,
    WHIP,
    HURT,
    JUMP,
    DEAD
};

// Container of a character's sprite

class Object2D : public GameObject
{
public:
    // Object state
    glm::vec2   		Position, Size, Velocity;
    GLboolean			Reversed;
    // column and line in the sprite sheet of the current displayed image
    State				Action; //line
    GLuint				N_img; //column
    std::vector<GLuint> N_max;// number of the images composing each animation (classed by line number)
    glm::vec2			Sprite_size; //width and height of one sprite in the texture, in a 0-1 range
    // Constructor(s)
    Object2D(Texture2D sprite, GLuint width, GLuint height);
    Object2D(Texture2D sprite, glm::vec2 position);
    Object2D(Texture2D sprite, std::vector<GLuint> N_max);
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
};