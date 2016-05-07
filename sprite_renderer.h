/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"


class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(const Shader &shader, GLboolean isskybox);
    SpriteRenderer(const Shader &shader, GLfloat tex_width, GLfloat tex_height);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(const Texture2D &texture, glm::vec3 position, glm::vec3 size = glm::vec3(1.0f), GLfloat rotate = 0.0f, glm::vec3 rotation_angle = glm::vec3(0.0f,0.0f,1.0f), glm::vec3 color = glm::vec3(1.0f), GLfloat alpha = 1.0f, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    void DrawSprite(const Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(1.0f), GLboolean reversed = GL_FALSE, GLuint line = 0, GLuint col = 0, glm::vec2 steps = glm::vec2(0), glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    void DrawSprite(const Texture3D &texture, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());

private:
    // Render state
    Shader    shader; 
    GLuint    quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
    void initSkyboxRenderData();
    void initRenderData(GLfloat tex_width, GLfloat tex_height);
};

#endif