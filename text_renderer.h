/*******************************************************************
** Taken and adapted from learnopengl.com (part of a Breakout game) 
******************************************************************/
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"
#include "state_manager.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};


// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer
{
public:
    // Holds a list of pre-compiled Characters
    std::map<GLchar, Character> Characters; 
    // Shader used for text rendering
    Shader TextShader;
    // Constructor
    TextRenderer(GLuint width, GLuint height);
    // Pre-compiles a list of characters from the given font
    void Load(std::string font, GLuint fontSize);
    // Renders a string of text using the precompiled list of characters
    void RenderText(StateManager &manager, std::string text, GLfloat x, GLfloat y, GLfloat scale = 1.0f, glm::vec3 color = glm::vec3(0));
private:
    // Render state
    GLuint VAO, VBO;
};

#endif 

