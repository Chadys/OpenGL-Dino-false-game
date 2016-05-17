/*******************************************************************
** Taken and adapted from learnopengl.com (part of a Breakout game) 
******************************************************************/
#include "sprite_renderer.h"


SpriteRenderer::SpriteRenderer(const Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}
SpriteRenderer::SpriteRenderer(const Shader &shader, std::pair<std::vector<glm::vec3>,std::vector<glm::vec3>> cubes_carac)
{
    this->shader = shader;
    this->initRenderData(cubes_carac);
}

SpriteRenderer::SpriteRenderer(const Shader &shader, GLfloat tex_width, GLfloat tex_height)
{
    this->shader = shader;
    this->initRenderData(tex_width, tex_height);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

// TEXTURED CUBE
void SpriteRenderer::DrawSprite(StateManager &manager, const Tex &texture, glm::vec3 position, glm::vec3 size, glm::vec3 rotation, glm::vec3 color, GLfloat alpha, glm::mat4 projection, glm::mat4 view)
{
    // Prepare transformations
    manager.Active(this->shader);
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position));  // First translate (transformations are: scale happens first, then rotation and then final translation happens; reversed order)

    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f,0.0f,0.0f)); // Then rotate
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f,0.0f,1.0f));
    
    model = glm::scale(model, glm::vec3(size)); // Last scale

    this->shader.SetMatrix4("model", model);
    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);

    this->shader.SetVector3f("spriteColor", color);
    this->shader.SetFloat("spriteAlpha", alpha);
    this->shader.SetInteger("is3D", GL_TRUE);

    glActiveTexture(GL_TEXTURE0);
    manager.ActiveTex2D(texture);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    this->shader.SetInteger("is3D", GL_FALSE);
}

// INSTANCED TEXTURED CUBE
void SpriteRenderer::DrawSprite(StateManager &manager, const Tex &texture, GLuint amount, glm::mat4 projection, glm::mat4 view)
{
    // Prepare transformations
    manager.Active(this->shader);

    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);

    this->shader.SetInteger("is3D", GL_TRUE);

    glActiveTexture(GL_TEXTURE0);
    manager.ActiveTex2D(texture);

    glBindVertexArray(this->quadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, amount);
    glBindVertexArray(0);
    this->shader.SetInteger("is3D", GL_FALSE);
}

// TEXTURED SQUARE
void SpriteRenderer::DrawSprite(StateManager &manager, const Tex &texture, glm::vec2 position, glm::vec2 size, GLboolean border, glm::mat4 projection, glm::mat4 view, GLfloat alpha, GLboolean reversed, GLuint line, GLuint col, glm::vec2 sprite_size)
{
    //Prepare transformations
    manager.Active(this->shader);
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position,0.0f));  

    if(reversed){
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f)); 
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    }

    model = glm::scale(model, glm::vec3(size, 0.0f)); 

    this->shader.SetMatrix4("model", model);
    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);

    this->shader.SetInteger("border", border);
    this->shader.SetFloat("spriteAlpha", alpha);
    this->shader.SetVector2uint("spritePos", col, line);
    this->shader.SetVector2f("spriteStep", sprite_size);
  
    glActiveTexture(GL_TEXTURE0);
    manager.ActiveTex2D(texture);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// COLORED SQUARE
void SpriteRenderer::DrawSprite(StateManager &manager, glm::vec2 position, glm::vec2 size, GLboolean isCircle, glm::vec3 color1, glm::vec3 color2, Effect effect, glm::mat4 projection, glm::mat4 view, GLboolean border)
{
    //Prepare transformations
    manager.Active(this->shader);
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position,0.0f));  

    model = glm::scale(model, glm::vec3(size, 0.0f)); 

    this->shader.SetMatrix4("model", model);
    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);

    this->shader.SetInteger("border", border);
    this->shader.SetInteger("isCircle", isCircle);
    this->shader.SetInteger("effect", effect);
    this->shader.SetVector3f("color1", color1);
    this->shader.SetVector3f("color2", color2);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    this->shader.SetInteger("effect", -1);
}

// SKYBOX
void SpriteRenderer::DrawSprite(StateManager &manager, const Tex &texture, glm::mat4 projection, glm::mat4 view)
{
    // Prepare transformations
    manager.Active(this->shader);

    glDepthFunc(GL_LEQUAL);
    view = glm::mat4(glm::mat3(view));
    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);

    glBindVertexArray(this->quadVAO);
    glActiveTexture(GL_TEXTURE0);
    manager.ActiveTex3D(texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

// FENCE CUBE
void SpriteRenderer::initRenderData(std::pair<std::vector<glm::vec3>,std::vector<glm::vec3>>cubes_carac)
{
    // Configure VAO/VBO
    GLuint VBO[3];

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO[0]);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    GLfloat vertices3d[] = {
        //FOND
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, //fond bas gauche
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, //fond haut droite
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //fond bas droite
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, //fond haut droite
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, //fond bas gauche
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, //fond haut gauche

        //DEVANT
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, //devant bas gauche
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, //devant bas droite
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, //devant haut droite
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, //devant haut droite
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //devant haut gauche
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, //devant bas gauche

        //GAUCHE
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, //devant haut gauche
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, //fond haut gauche
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, //fond bas gauche
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, //fond bas gauche
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, //devant bas gauche
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, //devant haut gauche
 
        //DROITE
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //devant haut droite
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //fond bas droite
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, //fond haut droite
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //fond bas droite
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //devant haut droite
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, //devant bas droite
 
        //BAS
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, //fond bas gauche
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, //fond bas droite
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, //devant bas droite
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, //devant bas droite
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, //devant bas gauche
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, //fond bas gauche

        //HAUT
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, //fond haut gauche
         0.5f,  0.5f,  0.5f,  0.0f, 0.5f, //devant haut droite
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, //fond haut droite
         0.5f,  0.5f,  0.5f,  0.0f, 0.5f, //devant haut droite
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, //fond haut gauche
        -0.5f,  0.5f,  0.5f,  1.0f, 0.5f  //devant haut gauche
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3d), &vertices3d, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    // TexCoord attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    // All positions for instanced drawing
    glGenBuffers(1, &VBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    glm::mat4 models[cubes_carac.first.size()];
    for(GLuint i = 0; i < cubes_carac.first.size(); i++){
        models[i] = glm::translate(glm::mat4(), cubes_carac.first[i]);
    }

    glBufferData(GL_ARRAY_BUFFER, cubes_carac.first.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);
    
    // Set attribute pointers for matrix (4 times vec4)
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    // All colors for instanced drawing
    glGenBuffers(1, &VBO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);

    glBufferData(GL_ARRAY_BUFFER, cubes_carac.second.size() * sizeof(glm::vec3), &cubes_carac.second[0], GL_STATIC_DRAW);
    
    // Set attribute pointers for matrix (4 times vec4)
    glEnableVertexAttribArray(7); 
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glVertexAttribDivisor(7, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// SKYBOX
void SpriteRenderer::initRenderData()
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

// SQUARE
void SpriteRenderer::initRenderData(GLfloat tex_width, GLfloat tex_height)
{
    // Configure VAO/VBO
    GLuint VBO;

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat vertices2d[] = {
        // Pos      // Tex
        0.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, tex_width,
        1.0f, 0.0f, tex_height, 0.0f, 
        1.0f, 1.0f, tex_width, tex_height
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2d), &vertices2d, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}