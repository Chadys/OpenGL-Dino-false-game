#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.h"
#include "texture.h"

// The different shaders possible
enum Active_Shader {
	NO_SHADER,
    JEU,
    MODEL,
    SKYBOX,
    TEXT
};

// The different texture possible
enum Active_Tex2D {
	NO_TEX,
    FENCE,
    DINO1,
    DINO2,
    BG,
    GRASS0,
    GRASS1,
    GRASS2,
    GRASS3,
    GRASS4,
    GRASS5,
    FERN,
    VINE,
    MENU
};

// Active fading effect
enum Fade_Effect {
	NO_FADE,
	FADIN,
	FADOUT
};

class StateManager{
public:
    Active_Shader  shader;	
    Active_Tex2D   tex2D;
    GLboolean      tex3D;
    GLfloat 	   fade;
    Fade_Effect    Fade;
    StateManager();
    void Active(Shader &_shader);
	virtual void ActiveTex2D(const Tex& texture);
	virtual void ActiveTex3D(const Tex& texture);
	void Update(GLfloat dt);
};