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
    // Set model's position correctly :
    // the given side of the model will be localised as to be at pos location if it is facing the orientation side_local
    // Putting the model in its correct orientation for it to work will need to be done elsewhere.
    void SetSide(GLfloat pos, Side side_local, Side side_model);
    void SetSide(GLfloat pos, Side side_local) { SetSide(pos, side_local, side_local); }
};