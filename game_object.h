#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "texture.h"
#include "sprite_renderer.h"
#include "model.h"

/*------------------------------------INTERFACES-----------------------------------------*/
class GameObject
{
public:
    // Render state
    Texture2D   Tex;	
    // Draw sprite
    GameObject(Texture2D sprite) : Tex(sprite) { }
    virtual void Draw(SpriteRenderer &renderer, glm::mat4 projection, glm::mat4 view) = 0;
};

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class Game_Object3D
{
public:
    // Object state
    glm::vec3   Position, Size, Color;
    GLfloat     Alpha, Rotation;
    glm::vec3   Rotation_angle;
    // Constructor(s)
    // Object3D();
    Game_Object3D(glm::vec3 pos, glm::vec3 size, glm::vec3 color = glm::vec3(1.0f))
     : Position(pos), Size(size), Color(color), Alpha(1.0f), Rotation(0.0f), Rotation_angle(0.0f, 1.0f, 0.0f) { }
};


/*------------------------------------MODEL CONTAINER-----------------------------------------*/

// Represents the different sides of a model
enum Side {
    UP_SIDE,
    DOWN_SIDE,
    FRONT_SIDE,
    BACK_SIDE,
    LEFT_SIDE,
    RIGHT_SIDE
};

// class regrouping property of a 3D model
class GameModel : public Game_Object3D
{
public:
    // Object state
    Model       model;
    // Constructor(s)
    GameModel(std::string file, std::string name);
    // Draw sprite
    void Draw(Shader shader, glm::mat4 projection, glm::mat4 view);
    // Set model's position correctly :
    // the given side of the model will be localised as to be at pos location if it is facing the orientation side_local
    // Putting the model in its correct orientation for it to work will need to be done elsewhere.
    void SetSide(GLfloat pos, Side side_local, Side side_model);
    void SetSide(GLfloat pos, Side side_local) { SetSide(pos, side_local, side_local); }
};



/*----------------------------------------CUBE---------------------------------------------*/

class Object3D : public GameObject, public Game_Object3D
{
public:
    // Constructor(s)
    Object3D(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f));
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
};



/*----------------------------------TEXTURED-SQUARE-----------------------------------------*/

// Container of a character's sprite

enum State {
    IDLE,
    WALK,
    BITE,
    WHIP,
    HURT,
    JUMP,
    DEAD
};

class Object2D : public GameObject
{
public:
    // Object state
    glm::vec2           Position, Size;
    GLboolean           Reversed;
    glm::vec2           Sprite_size; //width and height of one sprite in the texture, in a 0-1 range
    // Constructor(s)
    Object2D(Texture2D sprite, glm::vec2 position);
    Object2D(Texture2D sprite, glm::vec2 position, glm::vec2 size);
    Object2D(Texture2D sprite, std::vector<GLuint> N_max, GLfloat size_factor = 1.0f, glm::vec2 position = glm::vec2(0));
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    void         Update(GLfloat dt);
    void         SetState(State state);
    GLboolean    IsState(State state);
    glm::vec3 	 ExchangeSprite(const Object2D &sprite2, const glm::vec3 &cam_pos);
private:
    // column and line in the sprite sheet of the current displayed image
    State               Action; //line
    GLdouble            N_img; //column
    std::vector<GLuint> N_max;// number of the images composing each animation (classed by line number)
};


/*----------------------------------COLORED-SQUARE-----------------------------------------*/

class Square
{
public:
    // Object state
    glm::vec2   Position, Size, Velocity;
    GLboolean   IsCircle;
    glm::vec3   Color1, Color2;
    Effect      Effet;

    // Constructor(s)
    Square(glm::vec2 position, glm::vec2 size, glm::vec3 color1, glm::vec3 color2 = glm::vec3(0), Effect effect = NONE, glm::vec2 velocity = glm::vec2(0), GLboolean circle = GL_FALSE);
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4(), GLboolean border = GL_FALSE);
    void         Update(GLfloat dt);
};

#endif

