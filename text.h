#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "text_renderer.h"

class Text
{
public:
    std::string text;
    glm::vec2 Position;
    GLfloat Size;
    glm::vec3 Color;

    Text(std::string t, glm::vec2 position, GLfloat size = 1.0f, glm::vec3 color = glm::vec3(0));
    virtual void      Draw(TextRenderer &renderer);
};