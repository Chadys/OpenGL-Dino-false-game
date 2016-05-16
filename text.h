#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "text_renderer.h"
#include "state_manager.h"

class Text
{
public:
    std::string text;
    glm::vec2 Position;
    GLfloat Size_factor;
    glm::vec3 Color;

    Text(std::string t, glm::vec2 position, GLfloat size_factor = 1.0f, glm::vec3 color = glm::vec3(0));
    virtual void      Draw(StateManager &manager, TextRenderer &renderer);
};