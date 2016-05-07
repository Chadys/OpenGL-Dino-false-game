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

class Sprite
{
public:
    // Object state
    glm::vec2   		Position, Size, Velocity;
    GLboolean			Reversed;
    // Render state
    Texture2D   		Sheet;
    // column and line in the sprite sheet of the current displayed image
    State				Action; //line
    GLuint				N_img; //column
    std::vector<GLuint> N_max;// number of the images composing each animation (classed by line number)
    glm::vec2			Steps, Tex_coord; //width and height of one sprite in a 0-1 range for the sprite sheet and for the window
    // Constructor(s)
    Sprite(Texture2D sprite, GLuint game_width, GLuint game_height);
    Sprite(Texture2D sprite, GLuint game_width, GLuint game_height, std::vector<GLuint> N_max);
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
};