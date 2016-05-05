#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "model.h"

// Represents the different sides of a model
enum Side {
    UP_SIDE,
    DOWN_SIDE,
    FRONT_SIDE,
    BACK_SIDE,
    LEFT_SIDE,
    RIGHT_SIDE
};

// class regrouping property of a 3D model
class GameModel
{
public:
    // Object state
    glm::vec3   Position, Size, Velocity, Rotation_angle;
    GLfloat     Rotation;
    Model       model;
    // Constructor(s)
    GameModel(std::string file, std::string name);
    // Draw sprite
    void Draw(Shader shader, glm::mat4 projection, glm::mat4 view);
    // Set model's position correctly
    void SetSide(Side side, GLfloat pos);
};