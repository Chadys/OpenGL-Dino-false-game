

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Tex
{
public:
    // Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    GLuint ID;
    // Texture Format
    GLuint Internal_Format; // Format of texture object
    GLuint Image_Format; // Format of loaded image
    // Texture configuration
    GLuint Wrap_S; // Wrapping mode on S axis
    GLuint Wrap_T; // Wrapping mode on T axis
    GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
    GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
    // Constructor (sets default texture modes)
    Tex() : Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR_MIPMAP_LINEAR), Filter_Max(GL_LINEAR){
        glGenTextures(1, &this->ID);
    }
    virtual void Generate(GLuint width, GLuint height, unsigned char* data, GLuint i) = 0;
    // Binds the texture as the current active GL_TEXTURE_2D texture object
    virtual void Bind() const = 0;
};

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D : public Tex
{
public:
    // Texture image dimensions
    GLuint Width, Height; // Width and height of loaded image in pixels
    // Constructor (sets default texture modes)
    Texture2D();
    // Generates texture from image data
    virtual void Generate(GLuint width, GLuint height, unsigned char* data, GLuint i=0);
    // Binds the texture as the current active GL_TEXTURE_2D texture object
    virtual void Bind() const;
};

class Texture3D : public Tex
{
public:
    GLuint Wrap_R; // Wrapping mode on R axis
    // Constructor (sets default texture modes)
    Texture3D();
    // Generates texture from image data
    void Generate(GLuint width, GLuint height, unsigned char* data, GLuint i);
    // Binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};

#endif

