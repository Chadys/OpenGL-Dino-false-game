#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <map>
#include <string>
using namespace std;



// Container of a character's sprite

class Sprite
{
public:
    // Object state
    glm::vec3   							Position, Size, Velocity;
    GLboolean								reversed;
    // Render state
    Texture2D   							Sprite;
    // The position and size (x,y,w,h) in the sprite sheet corresponding to each state of a character (vec4) and the number of images of that state (GLuint) accessed by the name of the state.
    map<string, pair<glm::vec4,GLuint>>     States;
    //glm::vec4	Idle,Walk,Bite,Whip,Hit,Jump,Lose;
    // Constructor(s)
    GameObject();
    GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f));
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
};

#endif


    map<string, pair<glm::vec4,GLuint>>     States;
    pair<int,int> p1
	p1.first >> p1.second;