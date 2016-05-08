

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "texture.h"
#include "sprite_renderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // Render state
    Texture2D   Tex;	
    // Draw sprite
    GameObject(Texture2D sprite) : Tex(sprite) { }
    virtual void Draw(SpriteRenderer &renderer, glm::mat4 projection, glm::mat4 view) = 0;
};

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class Object3D : public GameObject
{
public:
    // Object state
    glm::vec3   Position, Size, Velocity, Color;
    GLfloat     Alpha, Rotation;
    glm::vec3   Rotation_angle;
    // Constructor(s)
    // Object3D();
    Object3D(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f));
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
};

// Container of a character's sprite

enum State {
    IDLE,
    WALK,
    BITE,
    WHIP,
    HURT,
    JUMP,
    DEAD
};

class Object2D : public GameObject
{
public:
    // Object state
    glm::vec2           Position, Size, Velocity;
    GLboolean           Reversed;
    glm::vec2           Sprite_size; //width and height of one sprite in the texture, in a 0-1 range
    // Constructor(s)
    Object2D(Texture2D sprite, GLuint width, GLuint height);
    Object2D(Texture2D sprite, glm::vec2 position);
    Object2D(Texture2D sprite, std::vector<GLuint> N_max);
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    void         Update(GLfloat dt);
    void         SetState(State state);
    GLboolean    IsState(State state);
private:
    // column and line in the sprite sheet of the current displayed image
    State               Action; //line
    GLdouble            N_img; //column
    std::vector<GLuint> N_max;// number of the images composing each animation (classed by line number)
};

#endif

