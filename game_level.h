/*******************************************************************
** Taken and adapted from learnopengl.com (part of a Breakout game) 
******************************************************************/
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
#include "resource_manager.h"
#include "texture.h"
#include "text.h"



/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
    // Level state
    std::vector<std::unique_ptr<GameObject>> Obj;
    std::unique_ptr<Tex> Bg;
    std::vector<Square> Squares;
    std::vector<Text> Texts;
    std::unique_ptr<TextRenderer> T_renderer;
    // Constructor
    GameLevel(std::unique_ptr<Tex> &bg);
    // Loads level from file
    void      Load(const GLchar *file);
    void      Load(const GLchar *file, GLuint width, GLuint height);
    void      LoadMenu(GLuint width, GLuint height);
    // Render level
    virtual void      Draw(SpriteRenderer &renderer, SpriteRenderer &bgrenderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    virtual void      Draw(SpriteRenderer &renderer, GLuint width, GLuint height, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4(), GLboolean menu = GL_FALSE);
    // Menu buttons state change
    void ActiveContinue();
    void ChangeButton(GLboolean up);
private:
    // Initialize level from tile data
    std::vector<std::vector<GLint>>      load(const GLchar *file);
    void                                 init(std::vector<std::vector<GLint>> tileData);
    void                                 init(std::vector<std::vector<GLint>> tileData, GLuint width, GLuint height);
    void                                 initMenu(GLfloat width, GLfloat height);
    void 								 Select(GLuint n);
    void 								 Deselect(GLuint n);
};

#endif

