#include "texture.h"

Texture3D::Texture3D()
    : Texture2D(), Wrap_R(GL_CLAMP_TO_EDGE) { }

void Texture3D::Generate(GLuint width, GLuint height, unsigned char* data, GLuint i)
{
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if(i==5){
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->Wrap_S);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->Wrap_T);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->Wrap_R);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }  
}

void Texture3D::Bind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
}