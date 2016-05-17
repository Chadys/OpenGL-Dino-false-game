#include "state_manager.h"

StateManager::StateManager() : shader(NO_SHADER), tex2D(NO_TEX), tex3D(GL_FALSE), fade(0), Fade(FADIN) { }

void StateManager::Active(Shader &_shader){
    if (this->shader != _shader.ID/3){
        this->shader = static_cast<Active_Shader>(_shader.ID/3);
        _shader.Use();
    }
    _shader.SetFloat("fade", this->fade);
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


void StateManager::Update(GLfloat dt){
	if (this->Fade == FADIN){
		this->fade += dt;
		if (this->fade > 1){
			this->fade = 1;
			this->Fade = NO_FADE;
		}
	}
	if (this->Fade == FADOUT){
		this->fade -= dt;
		if (this->fade < 0){
			this->fade = 0;
			this->Fade = FADIN;
		}
	}
}