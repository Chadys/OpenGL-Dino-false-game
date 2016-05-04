

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "sprite_renderer.h"


SpriteRenderer::SpriteRenderer(const Shader &shader, GLboolean isskybox, GLboolean is2d)
    : flat(is2d)
{
    this->shader = shader;
    if (!isskybox)
        this->initRenderData();
    else
        this->initSkyboxRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(const Texture2D &texture, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 rotation_angle, glm::vec3 color, GLfloat alpha, glm::mat4 projection, glm::mat4 view)
{
    // Prepare transformations
    this->shader.Use();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position));  // First translate (transformations are: scale happens first, then rotation and then final translation happens; reversed order)

    model = glm::rotate(model, glm::radians(rotate), rotation_angle); // Then rotate
    
    model = glm::scale(model, glm::vec3(size)); // Last scale

    this->shader.SetMatrix4("model", model);
    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);

    this->shader.SetVector3f("spriteColor", color);
    this->shader.SetFloat("spriteAlpha", alpha);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
void SpriteRenderer::DrawSprite(const Texture2D &texture, glm::vec3 position, glm::vec2 size, GLfloat rotate, glm::vec3 rotation_angle, glm::mat4 projection, glm::mat4 view)
{
    // Prepare transformations
    this->shader.Use();
    glm::mat4 model;
    model = glm::translate(model, position);  

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    model = glm::rotate(model, glm::radians(rotate), rotation_angle); 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f)); 

    this->shader.SetMatrix4("model", model);
    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);

    this->shader.SetVector3f("spriteColor", glm::vec3(1.0f));
    this->shader.SetFloat("spriteAlpha", 1.0f);
  
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(const Texture3D &texture, glm::mat4 projection, glm::mat4 view)
{
    // Prepare transformations
    this->shader.Use(); 

    glDepthFunc(GL_LEQUAL);
    view = glm::mat4(glm::mat3(view));
    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);

    glBindVertexArray(this->quadVAO);
    glActiveTexture(GL_TEXTURE0);
    this->shader.SetInteger("skybox", 0);
    texture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

void SpriteRenderer::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    if(this->flat){
        GLfloat vertices2d[] = { 
            // Pos      // Tex
            0.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 

            0.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2d), &vertices2d, GL_STATIC_DRAW);
    
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    }
    else{
        GLfloat vertices3d[] = {
            //FOND
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //fond bas gauche
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //fond haut droite
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //fond bas droite
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //fond haut droite
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //fond bas gauche
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //fond haut gauche

            //DEVANT
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //devant bas gauche
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //devant bas droite
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //devant haut droite
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //devant haut droite
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, //devant haut gauche
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //devant bas gauche

            //GAUCHE
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //devant haut gauche
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //fond haut gauche
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //fond bas gauche
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //fond bas gauche
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //devant bas gauche
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //devant haut gauche
 
            //DROITE
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f, //devant haut droite
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //fond bas droite
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //fond haut droite
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //fond bas droite
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f, //devant haut droite
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //devant bas droite
 
            //BAS
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //fond bas gauche
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //fond bas droite
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //devant bas droite
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //devant bas droite
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //devant bas gauche
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //fond bas gauche
 
            //HAUT
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //fond haut gauche
             0.5f,  0.5f,  0.5f,  1.0f, 0.5f, //devant haut droite
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //fond haut droite
             0.5f,  0.5f,  0.5f,  1.0f, 0.5f, //devant haut droite
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //fond haut gauche
            -0.5f,  0.5f,  0.5f,  0.0f, 0.5f  //devant haut gauche
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3d), &vertices3d, GL_STATIC_DRAW);

    // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    // TexCoord attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::initSkyboxRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat skyboxVertices[] = {
        // Positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
   
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
