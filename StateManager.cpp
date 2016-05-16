#include "state_manager.h"

StateManager::StateManager() : shader(NO_SHADER), tex2D(NO_TEX), tex3D(GL_FALSE) { }

void StateManager::Active(Shader &_shader){
    if (this->shader != _shader.ID/3){
        this->shader = static_cast<Active_Shader>(_shader.ID/3);
        _shader.Use();
    }
}

void StateManager::ActiveTex2D(const Tex& texture){
    if (this->tex2D != texture.ID/2+1){
        this->tex2D = static_cast<Active_Tex2D>(texture.ID/2+1);
        texture.Bind();
    }
}

void StateManager::ActiveTex3D(const Tex& texture){
    if (!this->tex3D){
        this->tex3D = GL_TRUE;
        texture.Bind();
    }
}