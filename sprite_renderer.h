/*******************************************************************
** Taken and adapted from learnopengl.com (part of a Breakout game) 
******************************************************************/
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "texture.h"
#include "shader.h"
#include "state_manager.h"

enum Effect {
    NONE,
    RADIAL_GRAD,
    V_GRAD,
    H_GRAD
};

class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(const Shader &shader);
    SpriteRenderer(const Shader &shader, std::pair<std::vector<glm::vec3>,std::vector<glm::vec3>> cubes_carac);
    SpriteRenderer(const Shader &shader, GLfloat tex_width, GLfloat tex_height);
    // Destructor
    virtual ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    virtual void DrawSprite(StateManager &manager, const Tex &texture, glm::vec3 position, glm::vec3 size = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0), glm::vec3 color = glm::vec3(1.0f), GLfloat alpha = 1.0f, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    virtual void DrawSprite(StateManager &manager, const Tex &texture, GLuint amount, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    virtual void DrawSprite(StateManager &manager, const Tex &texture, glm::vec2 position, glm::vec2 size, GLboolean border = GL_FALSE, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4(), GLfloat alpha = 1, GLboolean reversed = GL_FALSE, GLuint line = 0, GLuint col = 0, glm::vec2 sprite_size = glm::vec2(1));
    virtual void DrawSprite(StateManager &manager, const Tex &texture, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    void         DrawSprite(StateManager &manager, glm::vec2 position, glm::vec2 size, GLboolean isCircle, glm::vec3 color1, glm::vec3 color2, Effect effect, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4(), GLboolean border = GL_FALSE);

private:
    // Render state
    Shader    shader; 
    GLuint    quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData(std::pair<std::vector<glm::vec3>,std::vector<glm::vec3>> cubes_carac);
    void initRenderData();
    void initRenderData(GLfloat tex_width, GLfloat tex_height);
};

#endif